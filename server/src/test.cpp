#include "db.h"
#include "excercises.h"
#include "functions.h"
#include "tcpserver.h"
#include <QSignalSpy>
#include <QtTest/QtTest>

class Test : public QObject {
  Q_OBJECT

private slots:
  // MyTcpServer
  void testConstructor();
  void testSlotNewConnection();
  // void testSlotServerRead();
  void testSlotClientDisconnected();
  // DB
  void testOpen();
  void testInsertData();
  void testGetData();
  void testMakeInsertQuery();
  void testGetLastInsertId();
  // ServerFunctions
  void testSelectCommand();
  void testParse();
  void testRegisterUser();
  void testLoginUser();
  void testGetGrades();
  // void testCheckExercise();
};

// MyTcpServer
void Test::testConstructor() {
  MyTcpServer server;
  QCOMPARE(server.getServerStatus(), true);
}

void Test::testSlotNewConnection() {
  MyTcpServer server;
  QTcpSocket *socket = new QTcpSocket();
  QSignalSpy spy(socket, SIGNAL(connected()));
  socket->connectToHost(QHostAddress::LocalHost, 33333);
  spy.wait();
  QCOMPARE(spy.count(), 1);
  delete socket;
}

/*
void Test::testSlotServerRead() {
  MyTcpServer server;
  QTcpSocket *socket = new QTcpSocket();
  socket->connectToHost(QHostAddress::LocalHost, 33333);
  QByteArray testMessage = "Hello, World!\r\n";
  socket->write(testMessage);
  socket->waitForBytesWritten();
  socket->waitForReadyRead();
  QByteArray receivedMessage = socket->readAll();
  QCOMPARE(receivedMessage, ServerFunctions::parse(testMessage));
  delete socket;
}
*/

void Test::testSlotClientDisconnected() {
  MyTcpServer server;
  QTcpSocket *socket = new QTcpSocket();
  QSignalSpy spy(&server, SIGNAL(destroyed(QObject *)));
  socket->connectToHost(QHostAddress::LocalHost, 33333);
  socket->disconnectFromHost();
  spy.wait();
  QCOMPARE(spy.count(), 0);
  delete socket;
}

// DB
void Test::testOpen() {
  DB::getInstance().init();
  QCOMPARE(DB::getInstance().db.isOpen(), true);
  DB::getInstance().close();
}

void Test::testInsertData() {
  DB::getInstance().init();
  QString insertQuery =
      "INSERT INTO user (login, password, role_id) VALUES ('test', 'test', 1);";
  QCOMPARE(DB::getInstance().insertData(insertQuery), true);
  DB::getInstance().close();
}

void Test::testGetData() {
  DB::getInstance().init();
  QString selectQuery = "SELECT * FROM user WHERE login = 'test';";
  QMap<QString, QVariant> data = DB::getInstance().getData(selectQuery);
  QCOMPARE(data["login"].toString(), QString("test"));
  DB::getInstance().close();
}

void Test::testMakeInsertQuery() {
  DB::getInstance().init();
  QMap<QString, QMap<QString, QVariant>> data;
  QMap<QString, QVariant> userData;
  userData["login"] = "test2";
  userData["password"] = "test2";
  userData["role_id"] = 1;
  data["user"] = userData;
  QCOMPARE(DB::getInstance().makeInsertQuery(data), true);
  DB::getInstance().close();
}

void Test::testGetLastInsertId() {
  DB::getInstance().init();
  QString insertQuery = "INSERT INTO user (login, password, role_id) VALUES "
                        "('test3', 'test3', 1);";
  DB::getInstance().insertData(insertQuery);
  int lastId = DB::getInstance().getLastInsertId();
  QString selectQuery = QString("SELECT id FROM user WHERE login = 'test3';");
  QMap<QString, QVariant> data = DB::getInstance().getData(selectQuery);
  QCOMPARE(lastId, data["id"].toInt());
  DB::getInstance().close();
}

// ServerFunctions
void Test::testSelectCommand() {
  QJsonObject obj;
  obj["command"] = "invalidCommand";
  QJsonDocument doc(obj);
  QByteArray result = ServerFunctions::selectCommand(doc);
  QCOMPARE(result, QByteArray("Invalid command\n"));
}

void Test::testParse() {
  QString invalidJson = "{";
  QByteArray result = ServerFunctions::parse(invalidJson);
  QCOMPARE(result, QByteArray("Invalid JSON string\n"));
}

void Test::testRegisterUser() {
  QJsonObject obj;
  obj["login"] = "test";
  obj["password"] = "password";
  QByteArray result = ServerFunctions::registerUser(obj);
  QCOMPARE(result, QByteArray("Registration failed\n"));
}

void Test::testLoginUser() {
  QJsonObject obj;
  obj["login"] = "test";
  obj["password"] = "password";
  QByteArray result = ServerFunctions::loginUser(obj);
  QCOMPARE(result, QByteArray("Login failed\n"));
}

void Test::testGetGrades() {
  QJsonObject obj;
  obj["filter"] = "invalidFilter";
  QByteArray result = ServerFunctions::getGrades(obj);
  QCOMPARE(result, QByteArray("Invalid filter\n"));
}

/*
void Test::testCheckExercise() {
  QJsonObject obj;
  obj["student_id"] = 0;
  obj["exercise"] = 1;
  obj["answer"] = "answer";
  QByteArray result = ServerFunctions::checkExercise(obj);
  QCOMPARE(result, QByteArray("TODO\n"));
}
*/

QTEST_MAIN(Test)
#include "test.moc"
