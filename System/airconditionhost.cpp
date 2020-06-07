#include "AirConditionHost.h"
#include "UseDatabase.h"
#include "inovice.h"
#include "report.h"

AirConditionHost::AirConditionHost(QObject *parent):
    QObject(parent)
{
}

AirConditionHost::~AirConditionHost() {
    delete server;
}

void AirConditionHost::PowerOn() {
    server = new QTcpServer();
    db = new QSqlDatabase();
//	QString dsn = QString::fromLocal8Bit("ACCMS");      //数据源名称
//	db->setHostName("localhost");                       //选择本地主机，127.0.1.1
//	db->setDatabaseName(dsn);                           //设置数据源名称
//	db->setUserName("acc_manager");                     //登录用户
//	db->setPassword("123456");                          //密码
    CreatChartController();
    CreateMonitor();
    CreateSchduleController();
    CreateWaitList();
    CreateServiceList();
    qDebug()<<"请输入ManagerClient的端口：";
    QTextStream input(stdin);
    quint16 port;
    input >> port;
    connect(server, SIGNAL(newConnection()),
            this, SLOT(managerConnectHandle()));
    if(!server->listen(QHostAddress::Any, port))
    {
        qDebug()<<"服务器监听失败";
        return;
    }
    else
    {
        qDebug()<<"开始等待管理员";
    }
}

void AirConditionHost::managerConnectHandle() {
    QTcpSocket *managerSocket = server->nextPendingConnection();
    if(!managerSocket->isOpen())
        qDebug()<<"连接建立失败";
    else
        qDebug()<<"管理员已连接";
    chartConstroller->setSocket(managerSocket);
    disconnect(server, SIGNAL(newConnection()),
            this, SLOT(managerConnectHandle()));
    server->disconnect();
    server->close();
}

returnRequestOn AirConditionHost::CtreatClient(int Room_Id, double realTemp){
    AirConditionClient *client;
    client = new AirConditionClient();
    client->Initialize(Room_Id, mode, defaultTargetTemp, realTemp, defaultFeeRate, defaultFanSpeed, *db);
    waitList->PushACC(client);
    returnRequestOn r;
    r.RoomId = Room_Id;
    r.mode = mode;
    r.curTemp = realTemp;
    r.curFanSpeed = defaultFanSpeed;
    r.totalFee = 0;
    r.targetTemp = defaultTargetTemp;
    return r;
}

void AirConditionHost::setPara(double defaultTargetTemp, double maxTargetTemp, double minTargetTemp,
double highFeeRate, double middleFeeRate, double lowFeeRate, int mode, int speed){
    this->defaultTargetTemp = defaultTargetTemp;
    this->maxTargetTemp = maxTargetTemp;
    this->minTargetTemp = minTargetTemp;
    this->highFeeRate = highFeeRate;
    this->middleFeeRate = middleFeeRate;
    this->lowFeeRate = lowFeeRate;
    this->mode = mode;
    this->defaultFanSpeed = speed;
    switch (speed) {
    case 0:
        this->defaultFeeRate = lowFeeRate;
        break;
    case 1:
        this->defaultFeeRate = middleFeeRate;
        break;
    case 2:
        this->defaultFeeRate = highFeeRate;
        break;
    }
}

void AirConditionHost::startUp() {
    //system("cls");
    qDebug()<<"请输入GuestClientClient的端口：";
    QTextStream input(stdin);
    quint16 port;
    input >> port;
    connect(server, SIGNAL(newConnection()),
            this, SLOT(guestConnectHndle()));
    if(!server->listen(QHostAddress::Any, port))
    {
        qDebug()<<"服务器监听失败";
        return;
    }
    else{
        qDebug()<<"开始等待顾客";
    }
}

void AirConditionHost::guestConnectHndle(){
    qDebug()<<"有顾客客户端连接";
    QTcpSocket *socket = server->nextPendingConnection();
    scheduleController->addGuestSocket(socket);
}

void AirConditionHost::CreatChartController() {
    chartConstroller = new ChartController(this);
    chartConstroller->setAirConditionHost(this);
}

void AirConditionHost::CreateMonitor(){
    monitor = new Monitor();
}

void AirConditionHost::CreateSchduleController(){
    scheduleController = new ScheduleController(this);
    scheduleController->setAirConditionHost(this);
}

void AirConditionHost::CreateWaitList(){
    waitList = new WaitList();
	waitList->Initial();
}

void AirConditionHost::CreateServiceList(){
    serviceList = new ServiceList();
	serviceList->Initial();
}

int AirConditionHost::ChangeTargetTemp(int RoomID,float Temp)//设置温度 先在队列里面找 再去设置
{
    AirConditionClient* mclient;
    if (mclient = waitList->FindACC(RoomID)) {
        mclient->SetTargetTemp(Temp);
    }
    else if (mclient = waitList->FindACC(RoomID)) {
        mclient->SetTargetTemp(Temp);
    }
    UpdateChangeTempTime(RoomID,this->Date,*db);//db操作

}

int AirConditionHost:: ChangeFanSpeed(int RoomID,float Speed)//改变风速
{
    AirConditionClient* mclient;
    AirConditionClient* mVictimclient;
    AirConditionClient* mFrontclient;
    if (mclient = waitList->FindACC(RoomID))
    {
        if(!mclient->isRunning())//休眠
            mclient->SetSpeed(Speed);
        else {
			if (!serviceList->isEmpty() && mclient->GetPriority() > serviceList->GetMinPriority())//C:
            {
                mVictimclient = serviceList->GetAndPopVictim();//返回一个拷贝对象
                waitList->PushACC(mVictimclient);
                mVictimclient->StopRunning();
                mclient=waitList->PopACC(mclient->GetRoomId());//返回一个拷贝对象
                serviceList->PushACC(mclient);
                mclient->SetSpeed(Speed);
                mclient->StartRunning();
                UpdateSwitchOnOffTime(mclient->GetRoomId(),this->Date,*db);//db操作 开机
                UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度
            }
			else {//D:
                mclient->SetSpeed(Speed);
            }
        }
    }
    else if (mclient = serviceList->FindACC(RoomID))
    {
        if (!mclient->isRunning())
            mclient->SetSpeed(Speed);
        else {
            if(waitList->ReadyNum()==0)//E:
                mclient->SetSpeed(Speed);
            else if(mclient->GetPriority() < waitList->GetMaxPriority()){//B:
                mclient->SetSpeed(Speed);
                mclient->StopRunning();
                mclient=serviceList->PopACC(mclient->GetRoomId());//返回值是类的拷贝
                waitList->PushACC(mclient);
                mFrontclient=waitList->GetAndPopFrontACC();
                serviceList->PushACC(mFrontclient);
                mFrontclient->StartRunning();
                UpdateSwitchOnOffTime(mFrontclient->GetRoomId(),this->Date,*db);//db操作 开机


				//InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),mclient->Getstop_server_time(),mclient->GetTargetTemp(),mclient->GetFanSpeed(),mclient->GetFeeRate(),mclient->GetDuration(),mclient->GetFee(),*db);
                UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
                UpdateTotalFee(mclient->GetRoomId(),mclient->GetFee(),this->Date,*db);
                UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//一次详单 四件套

                UpdateChangeScheduleTime(mFrontclient->GetRoomId(),this->Date,*db);//db操作 发生调度
                UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度
            }
            else {//A:
                mclient->SetSpeed(Speed);
            }
        }
    }
    UpdateChangeFanSpeedTime(RoomID,this->Date,*db);//db操作
}

void AirConditionHost::ReachTargetTemperature(int RoomID)//达到目标后提出服务队列到等待队列 并给调度controller发消息
{
    AirConditionClient* mFrontclient;
    AirConditionClient* mclient= serviceList->PopACC(RoomID);
    mclient->StopRunning();
    if(waitList->ReadyNum() == 0)
    waitList->PushACC(mclient);
    else {
        waitList->PushACC(mclient);
        mclient->StopRunning();
        mFrontclient = waitList->GetAndPopFrontACC();
        serviceList->PushACC(mFrontclient);
        mFrontclient->StartRunning();
        UpdateSwitchOnOffTime(mFrontclient->GetRoomId(),this->Date,*db);//db操作 开机
    }
	//InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),mclient->Getstop_server_time(),mclient->GetTargetTemp(),mclient->GetFanSpeed(),mclient->GetFeeRate(),mclient->GetDuration(),mclient->GetFee(),*db);
    UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
    UpdateTotalFee(mclient->GetRoomId(),mclient->GetFee(),this->Date,*db);
    UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//一次详单 四件套


    UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度

}

void AirConditionHost::TimeOff(int RoomId,float FeeRate) {
    AirConditionClient* mclient = serviceList->FindACC(RoomId);
    AirConditionClient* mFrontclient;
    if (FeeRate > waitList->GetMaxPriority())
    {
        mclient->DestributeRunTime();
    }
    else {
        mclient = serviceList->PopACC(RoomId);
        mFrontclient = waitList->GetAndPopFrontACC();

        waitList->PushACC(mclient);
        mclient->StopRunning();
        serviceList->PushACC(mFrontclient);
        mFrontclient->StartRunning();
        mFrontclient->DestributeRunTime();

        UpdateSwitchOnOffTime(mFrontclient->GetRoomId(),this->Date,*db);//db操作 开机

		//InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),mclient->Getstop_server_time(),mclient->GetTargetTemp(),mclient->GetFanSpeed(),mclient->GetFeeRate(),mclient->GetDuration(),mclient->GetFee(),*db);
        UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
        UpdateTotalFee(mclient->GetRoomId(),mclient->GetFee(),this->Date,*db);
        UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//一次详单 四件套


        UpdateChangeScheduleTime(mFrontclient->GetRoomId(),this->Date,*db);//db操作 发生调度
        UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度
    }

}

void AirConditionHost::RequestService(int RoomId) {
    AirConditionClient* mclient = waitList->FindACC(RoomId);    //查找房间号对应的client
    AirConditionClient* mVictimclient;  //被牺牲的client
    if (!serviceList->isFull()) //服务队列未满
    {
        mclient = waitList->PopACC(RoomId);     //从等待队列弹出client
        serviceList->PushACC(mclient);  //服务队列加入mclient
        mclient->StartRunning();    //mclient开始运行
        mclient->DestributeRunTime();   //给mclient分配时间片
        UpdateSwitchOnOffTime(mclient->GetRoomId(),this->Date,*db);//db操作 开机
        UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度
    }
    else if(mclient->GetPriority()> serviceList->GetMinPriority()) {    //mclient的优先级大于服务队列中的最小优先级
        mVictimclient = serviceList->GetAndPopVictim();//找到牺牲者
        waitList->PushACC(mVictimclient);   //将牺牲者加入等待队列
        mVictimclient->StopRunning();   //牺牲者停止服务
        mclient = waitList->PopACC(mclient->GetRoomId());   //从等待队列移出mclient
        serviceList->PushACC(mclient);  //mclient加入服务队列
        mclient->StartRunning();    //mclient开始服务
        mclient->DestributeRunTime();   //mclient分配时间片

        UpdateSwitchOnOffTime(mclient->GetRoomId(),this->Date,*db);//db操作 开机

		//InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),mclient->Getstop_server_time(),mclient->GetTargetTemp(),mclient->GetFanSpeed(),mclient->GetFeeRate(),mclient->GetDuration(),mclient->GetFee(),*db);
        UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
        UpdateTotalFee(mclient->GetRoomId(),mclient->GetFee(),this->Date,*db);
        UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//记录详单

        UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度
        UpdateChangeScheduleTime(mVictimclient->GetRoomId(),this->Date,*db);//db操作 发生调度

    }
    else {
        waitList->PushACC(mclient);  //将请求客户端加入等待队列
    }
}

void AirConditionHost::CreateRDR(int RoomID, QString data_in, QString data_out)//请求数据库 返回详单指针
{
    Inovice detail(RoomID,*db);
   // mDetailRecords.PrintAsFile();//在屏幕上显示


}

void AirConditionHost::CreateReport(vector<int> listRoomId,int typeReport,QString  date)//请求数据库 返回报表指针
    {
    Report mReport=Report(date,*db);
//    QVector<int> roomid;
//    QVector<int> onOff;
//    QVector<int>  dura;
//    QVector<int> fee;
//    QVector<int> dispatch;
//    QVector<int> rdr;
//    QVector<int> temp;
//    QVector<int> speed;
//    for(auto &id : listRoomId)
//    {
//        QVector<struct report> temp=QueryDataInACCchart(date,*db);
//        roomid.push_back(Report.RoomId);
//        onOff.push_back(Report.SwitchOnoffTime);
//        fee.push_back(Report.TotalFee);
//        dispatch.push_back(Report.ScheduleTime);
//        rdr.push_back(Report.DetailRecordNum);
//        temp.push_back(Report.ChangeTempTime);
//        speed.push_back(Report.ChangeFanSpeedTime);
//    }
//    mReport.creat(roomid,  onOff , dura, fee,  dispatch, rdr, temp, speed);
//    mReport.PrintAsFile();//在屏幕上显示
    }

void AirConditionHost::CreateInvoice(int RoomID, QString data_in, QString data_out)//请求数据库 返回账单指针
{
    Inovice Invoices(data_in, data_out,  RoomID,*db);


}
