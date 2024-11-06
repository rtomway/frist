#include "LoginRegisterDlg.h"
#include"EventFilterObject.h"

#include<QPainter>
#include<QStaticText>
#include <QImage>
#include<QPushButton>
#include<QFile>
#include<QStackedWidget>
#include<QBoxLayout>
#include<QLineEdit>
#include<QCheckBox>
#include<QComboBox>
#include<QRadioButton>
#include<QSettings>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>

LoginRegisterDlg::LoginRegisterDlg(QWidget* parent)
	:QDialog(parent)
{
	initUi();
	readInfo();
}

void LoginRegisterDlg::initUi()
{
	//去掉标题栏
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
	//让无标题窗口可被拖动
	installEventFilter(new EventFilterObject(this));
	//让窗口透明
	setAttribute(Qt::WA_TranslucentBackground);
	

	resize(740, 470);

	//右
	m_rightRect = { 5,40,width() - 2 * 5,height() - 2 * 40 };
	//中
	m_midRect = { 5,5,370,440 };
	//左
	m_leftRect = { 4,5,187,440 };

	//提示框
	m_tipBox = new QLabel(this);
	m_tipBox->setGeometry(353, 390, 370, 30);
	m_tipBox->setAlignment(Qt::AlignCenter);
	setTip("请登录QAQ!");

	//关闭按钮
	auto closebtn = new QPushButton(this);
	closebtn->setGeometry(width() - 45, 45, 32, 32);
	closebtn->setObjectName("closebtn");
	connect(closebtn, &QPushButton::clicked, this, &QDialog::close);

	//堆栈窗口
	m_stkWidget = new QStackedWidget(this);
	m_stkWidget->setGeometry(434,150,200,240);
	m_stkWidget->setContentsMargins(0, 0, 0, 0);

	m_loginWidget = CreateLoginWidget();
	m_registerWidget = CreateRegisterWidget();
	m_stkWidget->addWidget(m_loginWidget);
	m_stkWidget->addWidget(m_registerWidget);

	//加载样式表
	QFile file(":/Resource/style.css");
	if (file.open(QIODevice::ReadOnly))
	{
		setStyleSheet(file.readAll());
	}
}

QWidget* LoginRegisterDlg::CreateLoginWidget()
{
	QWidget* w = new QWidget;

	m_login_accountEdit = new QLineEdit;
	 m_login_passwordEdit = new QLineEdit;
	 m_login_rememberpwd = new QCheckBox("记住密码");
	auto RegisterBtn = new QPushButton("注册账号");
	auto LoginBtn = new QPushButton("登录");

	m_login_accountEdit->setPlaceholderText("手机/邮箱");
	m_login_passwordEdit->setPlaceholderText("密码");

	m_login_rememberpwd->setObjectName("rememberpwd");
	RegisterBtn->setObjectName("RegisterBtn");
	LoginBtn->setObjectName("LoginBtn");

	connect(RegisterBtn, &QPushButton::clicked, [=]
		{
			m_stkWidget->setCurrentWidget(m_registerWidget);
		});

	connect(LoginBtn, &QPushButton::clicked,this,&LoginRegisterDlg::onLogin);

	auto vlayout = new QVBoxLayout;
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->addWidget(m_login_accountEdit);
	vlayout->addWidget(m_login_passwordEdit);

	auto hlayout = new QHBoxLayout;
	hlayout->addWidget(m_login_rememberpwd);
	hlayout->addWidget(RegisterBtn);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(LoginBtn);

	w->setLayout(vlayout);


	return w;
}

QWidget* LoginRegisterDlg::CreateRegisterWidget()
{
	QWidget* w = new QWidget;

	m_register_accountEdit = new QLineEdit;
	m_register_passwordEdit = new QLineEdit;
	m_register_repasswordEdit = new QLineEdit;
	m_register_manRBtn = new QRadioButton("男");
	m_register_womanRBtn = new QRadioButton("女");
	auto registerBtn = new QPushButton("注册账号");
	auto reLoginBtn = new QPushButton("返回登录");

	registerBtn->setObjectName("registerBtn");
	reLoginBtn->setObjectName("reLoginBtn");

	m_register_accountEdit->setPlaceholderText("手机/邮箱");
	m_register_passwordEdit->setPlaceholderText("密码");
	m_register_repasswordEdit->setPlaceholderText("重新输入密码");

	connect(reLoginBtn, &QPushButton::clicked, [=]
		{
			m_stkWidget->setCurrentWidget(m_loginWidget);
		});
	connect(registerBtn, &QPushButton::clicked, this, &LoginRegisterDlg::onRegister);

	auto hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(m_register_manRBtn);
	hlayout1->addWidget(m_register_womanRBtn);
	auto hlayout2 = new QHBoxLayout;
	hlayout2->setSpacing(0);
	hlayout2->addWidget(registerBtn);
	hlayout2->addWidget(reLoginBtn);

	auto vlayout = new QVBoxLayout(w);
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->addWidget(m_register_accountEdit);
	vlayout->addWidget(m_register_passwordEdit);
	vlayout->addWidget(m_register_repasswordEdit);
	vlayout->addLayout(hlayout1);
	vlayout->addLayout(hlayout2);

	return w;
}

void LoginRegisterDlg::saveInfo()
{
	

	QSettings settings("user.ini", QSettings::IniFormat);
	if (m_login_rememberpwd->isChecked())
	{
		settings.setValue("account", m_login_accountEdit->text());
		settings.setValue("password", m_login_passwordEdit->text());
		settings.setValue("rememberpwd", m_login_rememberpwd->isChecked());
	}
	else
	{
		settings.setValue("account","");
		settings.setValue("password","");
		settings.setValue("rememberpwd",false);
	}
}

void LoginRegisterDlg::readInfo()
{
	QSettings settings("user.ini", QSettings::IniFormat);
		m_login_accountEdit->setText(settings.value("account").toString());
		m_login_passwordEdit->setText(settings.value("password").toString());
		m_login_rememberpwd->setChecked(settings.value("rememberpwd").toBool());

}

void LoginRegisterDlg::setTip(const QString& msg)
{
	m_tipBox->setText("<font color=red>" + msg + "</font>");
}

void LoginRegisterDlg::onLogin()
{
	auto account = m_login_accountEdit->text();
	auto password = m_login_passwordEdit->text();
	auto isremember = m_login_rememberpwd->isChecked();

	if (account.isEmpty())
	{
		setTip("账户不能为空");
		return;
	}
	else if (password.isEmpty())
	{
		setTip("密码不能为空");
		return;
	}


	setTip("登录中...");

	//向服务器发送请求 验证信息
	QJsonObject jobj;
	jobj["account"] = account;
	jobj["password"] = password;

	 auto json=QJsonDocument(jobj).toJson(QJsonDocument::Compact);
	 qDebug() << json;

	 auto data = verifLogin(json);
	 QJsonParseError err;
	 QJsonDocument jd = QJsonDocument::fromJson(data.toLocal8Bit(), &err);
	 if (err.error != QJsonParseError::NoError)
	 {
		 return;
	 }
	 else
	 {
		 //进入主界面
		 qDebug() << data;
	 }
	//验证成功
	saveInfo();

}

void LoginRegisterDlg::onRegister()
{
	auto account = m_register_accountEdit->text();
	auto password = m_register_passwordEdit->text();
	auto repassword = m_register_repasswordEdit->text();
	auto gender= m_register_manRBtn->isChecked() ? 1 : (m_register_womanRBtn->isChecked()?2:0);

	if (account.isEmpty())
	{
		setTip("账户不能为空");
		return;
	}
	else if (password.isEmpty() || repassword.isEmpty())
	{
		setTip("密码不能为空");
		return;
	}

	if (password != repassword)
	{
		setTip("两次输入密码不一致");
		return;
	}
	setTip("注册中...");
	//生成json数据
	QJsonObject jobj;
	jobj["account"] = account;
	jobj["password"] = password;
	jobj["gender"] = gender;

	auto json = QJsonDocument(jobj).toJson(QJsonDocument::Compact);
	veriRegister(json);
	qDebug() << json;

}

void LoginRegisterDlg::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	//绘制右边白色矩形
	painter.setPen(Qt::NoPen);
	painter.fillRect(m_rightRect, Qt::white);
	//绘制左边矩形
	painter.fillRect(m_leftRect, qRgb(27, 45, 67));
	//绘制中间的弦
	painter.setBrush(QColor(27, 45, 67));
	painter.drawChord(m_midRect, -90 * 16, 180 * 16);

	//绘制welcome
	painter.setPen(Qt::white);
	painter.setFont(QFont("微软雅黑", 30));
	painter.drawStaticText(48, 60, QStaticText("Welcome!"));
	painter.setFont(QFont("微软雅黑", 10,QFont::Bold));
	painter.drawStaticText(48, 120, QStaticText("登录注册界面-x"));

	//粘贴“梦想”图片
	painter.drawPixmap(QRect(40, 200, 250, 250), QPixmap(":/Resource/cai.png"));
	painter.drawPixmap(QRect(40,200,250,250), QPixmap(":/Resource/dream.png"));
	painter.drawPixmap(QRect(40, 400, 50, 30), QPixmap(":/Resource/7.bmp"));

	//logo
	painter.drawPixmap(QPoint(423,70), QPixmap(":/Resource/dunkai.png"));
}

void LoginRegisterDlg::closeEvent(QCloseEvent* ev)
{
	saveInfo();
}

QString verifLogin(const QByteArray& json)
{
	QJsonParseError err;
	QJsonDocument jd = QJsonDocument::fromJson(json, &err);
	if (err.error != QJsonParseError::NoError)
	{
		return R"({"code":200,"message":"json 解析失败"})";
	}
	return R"({"code":200,"message":"success"})";
}

QString veriRegister(const QByteArray& json)
{
	return QString();
}
