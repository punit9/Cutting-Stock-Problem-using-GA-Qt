#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDebug>
#include<QClipboard>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void display(QString number);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int part=0;
    int partsize=35;
    int geneticParameter=100;
    bool alreadyran=false;
    int stockMaxIndex,cutMaxIndex;
    long long int scrap=0,minScrap=9999999999999,fullScrap=0,fullMinScrap=99999999999999;
    QList<int> minChromosome;
    QList<int> fullChromosome,fullMinChromosome;
    QList<int> stockItems,cutItems,stockQuantity,cutQuantity;
    QList<int> chromosome;
    QList<int> tempStockItems,tempCutItems,tempStockQuantity,tempCutQuantity;
    QList<int> minTempStockItems,currTempStockItems;
    QList<QList<int>> chromosomes;
    QList<int> fitness;
    QList<int> scraps;

    void reset();
    QList<int> nextCutItems(QList<int> *cutItems);
    void getTextData();
    int minfit(QList<int> t,int cutting=0);
    int maxfit(QList<int> t,int cutting=0);
    int max(QList<int> t);
    int min(QList<int> t);
    int generation();
    void fitnessCalc();
    void selection();
    void crossover();
    void mutation();
private slots:
    void on_optimizeButton_clicked();

//    void on_showPopulation_clicked();

//    void on_selection_clicked();

    void on_showResult_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
