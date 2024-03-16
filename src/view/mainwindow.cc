#include "mainwindow.h"

#include <QPixmap>

#include "qcustomplot.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  setWindowTitle("SmartCalc");
  setFixedSize(width(), height());
  SetTextToOut("0");
  InitializeConnections();

  // убираю выделяющуюся голубую рамку
  ui_->infix_expression->setAttribute(Qt::WA_MacShowFocusRect, 0);
  ui_->x_value_for_calc->setAttribute(Qt::WA_MacShowFocusRect, 0);
  ui_->xMin->setAttribute(Qt::WA_MacShowFocusRect, 0);
  ui_->xMax->setAttribute(Qt::WA_MacShowFocusRect, 0);
  ui_->yMin->setAttribute(Qt::WA_MacShowFocusRect, 0);
  ui_->yMax->setAttribute(Qt::WA_MacShowFocusRect, 0);

  ui_->widget_plot->xAxis->setLabel("x");
  ui_->widget_plot->yAxis->setLabel("y");

  ui_->xMin->setValidator(
      new QDoubleValidator(-1000000.0, 1000000.0, 7, ui_->xMin));
  ui_->xMax->setValidator(
      new QDoubleValidator(-1000000.0, 1000000.0, 7, ui_->xMax));
  ui_->yMin->setValidator(
      new QDoubleValidator(-1000000.0, 1000000.0, 7, ui_->yMin));
  ui_->yMax->setValidator(
      new QDoubleValidator(-1000000.0, 1000000.0, 7, ui_->yMax));
  ui_->x_value_for_calc->setValidator(
      new QDoubleValidator(-1000000.0, 1000000.0, 7, ui_->x_value_for_calc));

  ui_->infix_expression->setFocus();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::DigitsNum() {
  QString current_text = ui_->infix_expression->toPlainText();
  QPushButton *button = static_cast<QPushButton *>(sender());
  if (isNothingStr(current_text) || current_text == "+") current_text = "";
  if (button->text() == "X") {
    SetTextX(current_text);
  } else if (button->text() == "e") {
    SetTextE(current_text);
  } else if (button->text() == ".") {
    SetTextDot(current_text);
  } else {
    SetTextDigits(current_text, button->text());
  }
}

void MainWindow::Function() {
  QString current_text = ui_->infix_expression->toPlainText();
  QPushButton *button = static_cast<QPushButton *>(sender());
  QString button_text = button->text();
  if (isNothingStr(current_text)) current_text.clear();
  if (current_text.isEmpty())
    SetTextToOut(button_text + "(");
  else if ((current_text.length() > 1 && current_text.back().isDigit()) ||
           current_text.endsWith('x'))
    SetTextToOut(current_text + '*' + button_text + "(");
  else if (isOperator(current_text.back().toLatin1()))
    SetTextToOut(current_text + "(" + button_text + "(");
  else
    SetTextToOut(current_text + button_text + "(");
}

void MainWindow::Operators() {
  QString current_text = ui_->infix_expression->toPlainText();
  QPushButton *button = static_cast<QPushButton *>(sender());
  QString button_text = button->text();

  if (button_text == "×")
    button_text = '*';
  else if (button_text == "÷")
    button_text = '/';
  else if (button_text == "mod")
    button_text = '%';
  SetTextOperator(current_text, button_text);
}

void MainWindow::Brackets() {
  QString current_text = ui_->infix_expression->toPlainText();
  QPushButton *button = static_cast<QPushButton *>(sender());
  if (isNothingStr(current_text) || current_text == "+") current_text.clear();
  if (button->text() == "(") {
    SetTextOpenBrackets(current_text);
  } else if (button->text() == ")") {
    SetTextClosedBrackets(current_text);
  }
}

void MainWindow::Delete() {
  QString current_text = ui_->infix_expression->toPlainText();
  QPushButton *button = static_cast<QPushButton *>(sender());
  if (button->text() == "AC") {
    DeleteText();
  }
}

void MainWindow::ShowRes() {
  QPushButton *button = static_cast<QPushButton *>(sender());
  if (button->text() == "=") {
    Calculate();
  } else if (button->text() == "plot") {
    PlotGraph();
  }
}

void MainWindow::InitializeConnections() {
  connect(ui_->pushButton_0, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_1, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_2, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_3, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_4, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_5, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_6, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_7, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_8, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_9, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_X, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_E, SIGNAL(clicked()), this, SLOT(DigitsNum()));
  connect(ui_->pushButton_dot, SIGNAL(clicked()), this, SLOT(DigitsNum()));

  connect(ui_->pushButton_cos, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_tan, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_sin, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_atan, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_asin, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_acos, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_log, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_ln, SIGNAL(clicked()), this, SLOT(Function()));
  connect(ui_->pushButton_root, SIGNAL(clicked()), this, SLOT(Function()));

  connect(ui_->pushButton_plus, SIGNAL(clicked()), this, SLOT(Operators()));
  connect(ui_->pushButton_minus, SIGNAL(clicked()), this, SLOT(Operators()));
  connect(ui_->pushButton_mul, SIGNAL(clicked()), this, SLOT(Operators()));
  connect(ui_->pushButton_div, SIGNAL(clicked()), this, SLOT(Operators()));
  connect(ui_->pushButton_square, SIGNAL(clicked()), this, SLOT(Operators()));
  connect(ui_->pushButton_mod, SIGNAL(clicked()), this, SLOT(Operators()));

  connect(ui_->pushButton_open_bracet, SIGNAL(clicked()), this,
          SLOT(Brackets()));
  connect(ui_->pushButton_close_bracet, SIGNAL(clicked()), this,
          SLOT(Brackets()));

  connect(ui_->pushButton_AC, SIGNAL(clicked()), this, SLOT(Delete()));
  connect(ui_->pushButton_equal, SIGNAL(clicked()), this, SLOT(ShowRes()));

  connect(ui_->pushButton_make_plot, SIGNAL(clicked()), this,
          SLOT(PlotGraph()));

  //  connect(ui_->xMin, SIGNAL(editingFinished()), this, SLOT(axis_edited()));
  //  connect(ui_->xMax, SIGNAL(editingFinished()), this, SLOT(axis_edited()));
  //  connect(ui_->yMin, SIGNAL(editingFinished()), this, SLOT(axis_edited()));
  //  connect(ui_->yMax, SIGNAL(editingFinished()), this, SLOT(axis_edited()));
}

void MainWindow::axis_edited() {
  x_min = ui_->xMin->text().replace(",", ".").toDouble();
  x_max = ui_->xMax->text().replace(",", ".").toDouble();
  y_min = ui_->yMin->text().replace(",", ".").toDouble();
  y_max = ui_->yMax->text().replace(",", ".").toDouble();
  if (x_min < x_max && y_min < y_max) {
    ui_->widget_plot->xAxis->setRange(x_min, x_max);
    ui_->widget_plot->yAxis->setRange(y_min, y_max);
    ui_->widget_plot->replot();
  }
  PlotGraph();
}

bool MainWindow::isNothingStr(QString current_text) {
  return current_text.isEmpty() || current_text == "0" ||
         current_text == "Error" || current_text.endsWith("\u221E");
}

void MainWindow::SetTextX(QString current_text) {
  if (isNothingStr(current_text) || current_text == "+") current_text = "";
  if (current_text.isEmpty()) {
    SetTextToOut("x");
  } else if (current_text.back().isDigit() || current_text.back() == ')' ||
             current_text.back() == 'x') {
    SetTextToOut(current_text + "*x");
  } else if (!GetLastNumber(current_text).contains('e'))
    SetTextToOut(current_text + "x");
}

void MainWindow::SetTextToOut(QString string) {
  ui_->infix_expression->setText(string);
  ui_->infix_expression->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  QTextCursor cursor = ui_->infix_expression->textCursor();

  cursor.movePosition(QTextCursor::End);
  ui_->infix_expression->setTextCursor(cursor);
  ui_->infix_expression->setWordWrapMode(QTextOption::NoWrap);
  ui_->infix_expression->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

QString MainWindow::GetLastNumber(QString current_text) {
  int last_number_start = -1;
  for (int i = current_text.length() - 1; i >= 0; i--) {
    QChar ch = current_text[i];
    if (ch.isDigit() || ch == '.' || ch == 'e' ||
        (ch == '-' && current_text[i - 1] == 'e')) {
      last_number_start = i;
    } else
      break;
  }
  if (last_number_start >= 0) return current_text.mid(last_number_start);
  return "";
}

void MainWindow::SetTextE(QString current_text) {
  QString last_number = GetLastNumber(current_text);
  if (!current_text.isEmpty() && current_text.back().isDigit() &&
      !last_number.contains('e'))
    SetTextToOut(current_text + "e");
}

void MainWindow::SetTextDot(QString current_text) {
  if (current_text.isEmpty() || isOperator(current_text.back().toLatin1()) ||
      current_text.endsWith('('))
    SetTextToOut(current_text + "0.");
  else {
    QString last_number = GetLastNumber(current_text);
    if (!last_number.contains('.') && !last_number.contains('e'))
      SetTextToOut(current_text + ".");
  }
}

void MainWindow::SetTextDigits(QString current_text, QString button_text) {
  if (isNothingStr(current_text) || current_text == "+") current_text = "";
  if (current_text.endsWith(')') || current_text.endsWith('x')) {
    SetTextToOut(current_text + "*" + button_text);
  } else if (button_text == "0" && current_text.endsWith('(')) {
    SetTextToOut(current_text + button_text + ".");
  } else if ((current_text.endsWith('+') && current_text.length() >= 2 &&
              *(current_text.rbegin() + 1) == QChar('(')) ||
             current_text == "+") {
    current_text.chop(1);
    SetTextToOut(current_text + button_text);
  } else {
    SetTextToOut(current_text + button_text);
  }
}

bool MainWindow::isOperator(char ch) {
  std::array<char, 7> operators = {'+', '-', '*', '/', '^', '%', '^'};
  return std::any_of(operators.begin(), operators.end(),
                     [ch](char c) { return c == ch; });
}

void MainWindow::SetTextOperator(QString current_text, QString button_text) {
  if (isNothingStr(current_text))
    current_text = "";
  else if (isOperator(current_text.back().toLatin1()) ||
           current_text.back() == '.')
    current_text.chop(1);

  if (current_text.isEmpty() || current_text.endsWith('(') ||
      current_text.endsWith('e')) {
    if (button_text == "+" || button_text == "-")
      SetTextToOut(current_text + button_text);
  } else {
    if (current_text.back().isDigit() || current_text.endsWith('x') ||
        current_text.endsWith(')'))
      SetTextToOut(current_text + button_text);
  }
}

void MainWindow::SetTextOpenBrackets(QString current_text) {
  if (isNothingStr(current_text) || current_text == "+") current_text = "";
  if (current_text.isEmpty()) {
    SetTextToOut("(");
  } else if (isOperator(current_text.back().toLatin1()) ||
             current_text.back() == '(') {
    SetTextToOut(current_text + '(');
  } else if (!(current_text.endsWith('('))) {
    SetTextToOut(current_text + "*(");
  }
}

void MainWindow::SetTextClosedBrackets(QString current_text) {
  if (current_text.count('(') > current_text.count(')') &&
      !current_text.endsWith('(') &&
      (current_text.back().isDigit() || current_text.endsWith('x') ||
       current_text.endsWith(')')))
    SetTextToOut(current_text + ')');
}

void MainWindow::DeleteText() {
  ui_->x_value_for_calc->clear();
  SetTextToOut("0");
}

void MainWindow::Calculate() {
  QString current_text = AddCloseBrackets();
  bool x_value_convert;
  long double x_value =
      ui_->x_value_for_calc->text().toDouble(&x_value_convert);
  int count_x = current_text.count('x');
  if ((count_x == 0 && !x_value_convert) || (count_x > 0 && x_value_convert) ||
      current_text.length() <= 255) {
    calculator_controller_.SetExpersion(current_text.toStdString(), x_value);
    calculator_controller_.Calculate();
    if (calculator_controller_.GetStatus() == s21::ok) {
      long double result = calculator_controller_.GetResult();
      SetTextToOut(ChopZero(result));
      if (ui_->infix_expression->toPlainText().startsWith("nan"))
        SetTextToOut("Error");
      else if (ui_->infix_expression->toPlainText().endsWith("-inf"))
        SetTextToOut("-\u221E");
      else if (ui_->infix_expression->toPlainText().endsWith("inf"))
        SetTextToOut("\u221E");
    } else
      SetTextToOut("Error");
  } else if (count_x > 0 && !x_value_convert) {
    QMessageBox::information(this, "ERROR", "Set value of x correctly");
  } else
    SetTextToOut("Error");
}

QString MainWindow::AddCloseBrackets() {
  QString current_text = ui_->infix_expression->toPlainText();
  while (current_text.count('(') > current_text.count(')') &&
         current_text.back() != '(')
    current_text += ')';
  return current_text;
}

QString MainWindow::ChopZero(long double result) {
  QString res_string = QString::number(result, 'f', 6);
  while (res_string.endsWith('0')) res_string.chop(1);
  if (res_string.endsWith('.') || res_string.endsWith(',')) res_string.chop(1);
  return res_string;
}

// Построение графика

void MainWindow::Plot(QString expersion, QString x_value_str) {
  x_vector_.clear();
  y_vector_.clear();
  ui_->widget_plot->clearGraphs();

  bool convert_x_min_status, convert_x_max_status;
  bool convert_y_min_status, convert_y_max_status;
  //  x_begin_ = ui_->xMin->toPlainText().toDouble(&convert_x_min_status);
  x_begin_ = ui_->xMin->text().toDouble(&convert_x_min_status);
  x_end_ = ui_->xMax->text().toDouble(&convert_x_max_status);
  y_begin_ = ui_->yMin->text().toDouble(&convert_y_min_status);
  y_end_ = ui_->yMax->text().toDouble(&convert_y_max_status);
  if (!convert_x_min_status || !convert_x_max_status || !convert_y_min_status ||
      !convert_y_max_status) {
    QMessageBox::information(this, "ERROR", "Set domain & codomain correctly");
  } else if (x_begin_ < -1000000 || x_end_ > 1000000 || y_begin_ < -1000000 ||
             y_end_ > 1000000) {
    QString error = "Set parameters in the range from -1000000 to 1000000";
    QMessageBox::information(this, "ERROR", error);
  } else {
    bool convert_x_value_status;
    long double x_value = x_value_str.toDouble(&convert_x_value_status);
    if (convert_x_value_status) {
      for (x_cord_ = x_begin_; x_cord_ <= x_end_; x_cord_ += kStep_) {
        calculator_controller_.SetExpersion(expersion.toStdString(), x_cord_);
        calculator_controller_.Calculate();
        y_cord_ = calculator_controller_.GetResult();
        if (y_cord_ >= y_begin_ && y_cord_ <= y_end_ &&
            calculator_controller_.GetStatus() == s21::ok) {
          x_vector_.push_back(x_value);
          y_vector_.push_back(y_cord_);
        }
      }
    } else {
      for (x_cord_ = x_begin_; x_cord_ <= x_end_; x_cord_ += kStep_) {
        calculator_controller_.SetExpersion(expersion.toStdString(), x_cord_);
        calculator_controller_.Calculate();
        y_cord_ = calculator_controller_.GetResult();
        if (y_cord_ >= y_begin_ && y_cord_ <= y_end_ &&
            calculator_controller_.GetStatus() == s21::ok) {
          x_vector_.push_back(x_cord_);
          y_vector_.push_back(y_cord_);
        }
      }
    }

    ui_->widget_plot->addGraph();
    QCPGraph *Graph = ui_->widget_plot->graph(0);
    QPen pen = Graph->pen();
    pen.setWidth(2);
    pen.setColor(QColor::fromRgb(71, 86, 121));
    Graph->setPen(pen);
    ui_->widget_plot->graph(0)->addData(x_vector_, y_vector_);
    //                ui_->widget_plot->setInteractions(QCP::iRangeDrag |
    //                QCP::iRangeZoom | QCP::iSelectPlottables);
    ui_->widget_plot->rescaleAxes();
    ui_->widget_plot->replot();
  }
}

void MainWindow::PlotGraph() {
  QString current_text = AddCloseBrackets();
  SetTextToOut(current_text);
  QString x_value_str = ui_->x_value_for_calc->text();
  //  emit
  Plot(current_text, x_value_str);
}

void MainWindow::ChopText(QString current_text) {
  if (current_text.length() > 1) {
    current_text.chop(1);
    while (!current_text.isEmpty() &&
           ((current_text.back().toLatin1() >= 97 &&
             current_text.back().toLatin1() <= 122 &&
             current_text.back().toLatin1() != 'x') ||
            current_text.endsWith('E')))
      current_text.chop(1);
    SetTextToOut(current_text);
  } else
    SetTextToOut("0");
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  QString current_text = ui_->infix_expression->toPlainText();
  int key = event->key();
  if (key >= Qt::Key_0 && key <= Qt::Key_9) {
    QString key_text = QString::number(key - Qt::Key_0);
    SetTextDigits(current_text, key_text);
  } else if (key == Qt::Key_Period || key == Qt::Key_Comma) {
    SetTextDot(current_text);
  } else if (key == Qt::Key_X) {
    SetTextX(current_text);
  } else if (key == Qt::Key_E) {
    SetTextE(current_text);
  } else if (key == Qt::Key_Plus) {
    SetTextOperator(current_text, "+");
  } else if (key == Qt::Key_Minus) {
    SetTextOperator(current_text, "-");
  } else if (key == Qt::Key_Asterisk) {
    SetTextOperator(current_text, "*");
  } else if (key == Qt::Key_Slash) {
    SetTextOperator(current_text, "/");
  } else if (key == Qt::Key_Percent) {
    SetTextOperator(current_text, "%");
  } else if (key == Qt::Key_AsciiCircum) {
    SetTextOperator(current_text, "^");
  } else if (key == Qt::Key_ParenLeft) {
    SetTextOpenBrackets(current_text);
  } else if (key == Qt::Key_ParenRight) {
    SetTextClosedBrackets(current_text);
  } else if (key == Qt::Key_Delete) {
    DeleteText();
  } else if (key == Qt::Key_Backspace) {
    ChopText(current_text);
  } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
    if (ui_->pushButton_equal->text() == "=")
      Calculate();
    else if (ui_->pushButton_make_plot->text() == "plot")
      PlotGraph();
  }
}
