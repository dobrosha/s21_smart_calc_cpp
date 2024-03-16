#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QKeyEvent>
#include <QMainWindow>
#include <QString>
// Построение графика
#include <QDialog>
#include <QVector>

#include "./../controller/s21_calc_controller.h"

#define DOTS_COUNT 700

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  void output_answer(double res, int error_code);
  std::string x, expression;
  double x_min, x_max, y_min, y_max;
  int dots_count = DOTS_COUNT;
  double step;
  double x_vector[DOTS_COUNT];
  double y_vector[DOTS_COUNT];

  bool isNothingStr(QString current_text);
  void SetTextDigits(QString current_text, QString button_text);
  void SetTextDot(QString current_text);
  void SetTextX(QString current_text);
  void SetTextE(QString current_text);
  void SetTextToOut(QString string);
  QString ChopZero(long double result);

  bool isOperator(char ch);
  void SetTextOperator(QString current_text, QString button_text);
  QString AddCloseBrackets();

  void SetTextOpenBrackets(QString current_text);
  void SetTextClosedBrackets(QString current_text);

  void DeleteText();
  void ChopText(QString current_text);

  void Calculate();

  QString GetLastNumber(QString current_text);

  s21::CalculatorController calculator_controller_;

  void InitializeConnections();

  Ui::MainWindow *ui_;

  // Построение графика
  const double kStep_ = 0.1;
  double x_begin_ = -10;
  double x_end_ = 10;
  double y_begin_ = -10;
  double y_end_ = 10;
  double x_cord_;
  double y_cord_;
  QVector<double> x_vector_;
  QVector<double> y_vector_;

 private slots:
  void DigitsNum();
  void Function();
  void Operators();
  void Brackets();
  void Delete();
  void ShowRes();

  // Построение графика
  void Plot(QString expersion, QString x_value);
  void PlotGraph();
  void axis_edited();
};
#endif  // MAINWINDOW_H
