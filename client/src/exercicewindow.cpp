#include "exercicewindow.h"
#include "ui_exercicewindow.h"
#include <QPair>
#include <QRandomGenerator>
#include <QVector>
#include <algorithm>
#include <qstringliteral.h>
#include <random>

ExerciceWindow::ExerciceWindow(QWidget *parent, int excerciseNumber)
    : QMainWindow(parent), ui(new Ui::ExerciceWindow) {
  ui->setupUi(this);
  connect(ui->answerButton, SIGNAL(clicked()), this,
          SLOT(onAnswerButtonClick()));
  excerciseId = excerciseNumber;
  client = Client::getInstance();
  connect(client, SIGNAL(handleMessage(QString)), this,
          SLOT(exerciseHandeler(QString)));
  ui->errorLabel->setVisible(false);
  ui->errorLabel->setStyleSheet("QLabel { color : red; }");
  ui->errorLabel->setText("Неверный ответ");
  setExcercise();
}

ExerciceWindow::~ExerciceWindow() { delete ui; }

void ExerciceWindow::setExcercise() {
  // excerciseText - описание задачи
  // excerciseData - данные для задачи для ui
  // excerciseRawData - данные для задачи для сервера (сюда то что от вашего
  // генератора засунуть надо будет)
  switch (excerciseId) {

  case 1: {
    // Установка текста задачи
    ui->excerciseText->setText("Постройте минимальный каркас графа.");

    // Генерация данных для задачи
    int numNodes = QRandomGenerator::global()->bounded(4, 10);
    QString generatedData = QString::number(numNodes) + " ";

    QList<QPair<int, int>> edges;
    for (int i = 1; i < numNodes; ++i) {
      int node1 = QRandomGenerator::global()->bounded(i);
      int node2 = i;
      edges.append(qMakePair(node1, node2));
      generatedData +=
          QString::number(node1) + " " + QString::number(node2) + " ";
    }

    // Сохранение данных для задачи чтобы потом кинуть в сервер
    excerciseRawData = generatedData.trimmed();
    qDebug() << excerciseRawData;

    // Форматирование данных для ui
    QString formattedExcerciseData;
    QStringList tokens = generatedData.split(' ');
    if (!tokens.isEmpty()) {
      int numNodes = tokens.takeFirst().toInt();
      formattedExcerciseData += QString::number(numNodes) + " ";

      while (tokens.size() >= 2) {
        int node1 = tokens.takeFirst().toInt();
        int node2 = tokens.takeFirst().toInt();
        formattedExcerciseData += "(" + QString::number(node1) + ", " +
                                  QString::number(node2) + "), ";
      }

      if (!formattedExcerciseData.isEmpty()) {
        formattedExcerciseData.chop(2);
      }
    }

    // Установка данных для задачи в ui
    ui->excerciseData->setText(formattedExcerciseData.trimmed());
  } break;

  case 2:
    ui->excerciseText->setText("2");
    break;
  case 3: {
    // Установка текста задачи
    ui->excerciseText->setText("Составьте код прюффера для графа:");

    // Генерация данных для задачи
    int numNodes = QRandomGenerator::global()->bounded(4, 10);
    QString generatedData = QString::number(numNodes) + " ";

    QList<int> nodes;
    for (int i = 0; i < numNodes; ++i) {
      nodes.append(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nodes.begin(), nodes.end(), g);

    for (int i = 1; i < numNodes; ++i) {
      int parent = nodes.at(QRandomGenerator::global()->bounded(i));
      int child = nodes.at(i);
      generatedData +=
          QString::number(parent) + " " + QString::number(child) + " ";
    }

    // Сохранение данных для задачи чтобы потом кинуть в сервер
    excerciseRawData = generatedData.trimmed();
    qDebug() << excerciseRawData;

    // Форматирование данных для для ui
    QString formattedExcerciseData;
    QStringList tokens = generatedData.split(' ');
    if (!tokens.isEmpty()) {
      int numNodes = tokens.takeFirst().toInt();
      formattedExcerciseData += QString::number(numNodes) + " ";

      while (tokens.size() >= 2) {
        int node1 = tokens.takeFirst().toInt();
        int node2 = tokens.takeFirst().toInt();
        formattedExcerciseData += "(" + QString::number(node1) + ", " +
                                  QString::number(node2) + "), ";
      }

      if (!formattedExcerciseData.isEmpty()) {
        formattedExcerciseData.chop(2);
      }
    }

    // установка данных для задачи в ui
    ui->excerciseData->setText(formattedExcerciseData.trimmed());
  } break;
  case 4: {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(5, 15);

    int vertexesNum = dis(gen);
    QList<int> prueferCode;

    for (int num = 1; num <= vertexesNum; num++) {
      prueferCode.append(num);
    }
    std::shuffle(prueferCode.begin(), prueferCode.end(), gen);
    prueferCode.removeLast();
    prueferCode.removeLast();

    QString prueferCodeData;

    for (int value : prueferCode) {
      prueferCodeData += QString::number(value) + " ";
    }

    prueferCodeData.chop(1);
    excerciseRawData = prueferCodeData;
    ui->excerciseText->setText("Декодируйте код Прюффера");
    ui->excerciseData->setText(
        prueferCodeData.trimmed() +
        ", \n где идут номера вершин через пробел. \nОтвет запишите в формате "
        "\"x y, z w, ... \" ");
  } break;
  case 5: {
    ui->excerciseText->setText("Найдите максимальный поток в графе:");
    int numVertices =
        QRandomGenerator::global()->bounded(4, 7); // Количество вершин в графе
    QVector<int> data;

    // Генерация случайных ребер и их пропускных способностей
    for (int from = 0; from < numVertices; ++from) {
      for (int to = from + 1; to < numVertices; ++to) {
        if (to != numVertices - 1 && from != 0 &&
            QRandomGenerator::global()->bounded(0, 2) == 0)
          continue; // 50%
        int capacity = QRandomGenerator::global()->bounded(4, 10);
        data.append(from);
        data.append(to);
        data.append(capacity);
      }
    }

    // Формирование строки в требуемом формате
    QString inputString;
    inputString.append(QString::number(numVertices));

    for (int i = 0; i < data.size(); ++i) {
      inputString.append(" ");
      inputString.append(QString::number(data[i]));
    }

    excerciseRawData = inputString;

    QStringList tokens = excerciseRawData.split(' ');

    QString formattedData;
    formattedData = tokens[0] + " ";
    for (int i = 1; i < tokens.size(); i += 3) {
      QString from = tokens[i];
      QString to = tokens[i + 1];
      QString capacity = tokens[i + 2];

      QString edge = "(" + from + ", " + to + ", " + capacity + ")";
      formattedData.append(edge);

      if (i != tokens.size() - 3) {
        formattedData.append(" ");
      }
    }

    ui->excerciseData->setText(formattedData);

  } break;
  }
}

void ExerciceWindow::onAnswerButtonClick() {
  QString answer = ui->answerLineEdit->text();
  client->checkExercise(excerciseId, answer, excerciseRawData);
}

void ExerciceWindow::exerciseHandeler(QString response) {
  if (response == "Incorrect answer\n") {
    ui->errorLabel->setVisible(true);
  } else if (response == "Correct answer\n") {
    this->close();
  }
}
