#ifndef _LOGINREGISTERDLG_H_
#define _LOGINREGISTERDLG_H_

#include<QDialog>
#include<QStackedWidget>
#include<QLabel>
#include<QLineEdit>
#include<QCheckBox>
#include<QComboBox>
#include<QRadioButton>

class LoginRegisterDlg :public QDialog
{
public:
	LoginRegisterDlg(QWidget* parent = nullptr);
protected:
	void paintEvent(QPaintEvent* ev)override;
	void closeEvent(QCloseEvent* ev)override;
private:
	void initUi();
	QWidget* CreateLoginWidget();
	QWidget* CreateRegisterWidget();

	void saveInfo();
	void readInfo();

	QRect m_rightRect;
	QRect m_midRect;
	QRect m_leftRect;
	QWidget* m_loginWidget{};
	QWidget* m_registerWidget{};
	QStackedWidget* m_stkWidget{};
	QLabel *m_tipBox{};
	void setTip(const QString& msg);

	QLineEdit* m_login_accountEdit{};
	QLineEdit* m_login_passwordEdit{};
	QCheckBox* m_login_rememberpwd{};


	QLineEdit* m_register_accountEdit{};
	QLineEdit* m_register_passwordEdit{};
	QLineEdit* m_register_repasswordEdit{};
	QRadioButton* m_register_manRBtn{};
	QRadioButton* m_register_womanRBtn{};


public://slots
	void onLogin();
	void onRegister();

};

//模拟服务器验证
QString verifLogin(const QByteArray& json);
QString veriRegister(const QByteArray& json);

#endif // !_LOGINREGISTERDLG_H_
