#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->hide();
    fitness.append(1);
    chromosomes.append(QList<int>());
    scraps.append(1);
    connect(this,SIGNAL(display(QString)),ui->scrapNumber,SLOT(display(QString)));

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
        scrap=0;//Later clear all here
        chromosome.clear();
        for (int i=0;i<tempCutItems.length() ; i++ ) {
            chromosome.append(-1);
        }

//    cutItems<<1350<<1350<<1250<<1150<<1150<<1150<<1050<<950<<850<<850<<750<<750<<650<<550<<550<<450<<250;
//    stockItems<<3300<<3300<<3300<<3300<<3300<<3300<<3300<<3300<<3300<<3300<<3000<<3000<<3000<<3000<<3000;

}

QList<int> MainWindow::nextCutItems(QList<int> *cutItems)
{
    if(part*partsize<=cutItems->length()-1){
        return cutItems->mid(part*partsize,partsize);
    }
    return QList<int>();
}

void MainWindow::getTextData()
{
    QString text=ui->textData->toPlainText();
    qDebug()<<text;
    QList<QStringList> lines;
    foreach(QString line, text.split('\n')){
        lines.append(line.simplified().split(" "));
    }

    foreach(QStringList line,lines){
        if(line.length()>=2)
        if(!line.at(0).simplified().isEmpty())
        {
            long length=line.at(0).simplified().toLong();
            int times=line.at(1).simplified().toInt();
//            cutItems.append(length);
//            cutQuantity.append(times);

            for(int i=0;i<times;i++)//OLD LARGE LIST CODE
            cutItems.append(length);
        }
        if(line.length()>=4)
        if(!line.at(2).simplified().isEmpty()){
            long length=line.at(2).simplified().toLong();
            int times=line.at(3).simplified().toInt();
//            stockItems.append(length);

            for(int i=0;i<times;i++) //OLD LARGE LIST CODE
            stockItems.append(length);

//            stockQuantity.append(times);
        }
    }

//    std::random_shuffle(cutItems.begin(),cutItems.end());
    qDebug()<<cutItems;
    minTempStockItems=stockItems;
    qDebug()<<"CUT ITEMS : "<<cutItems<<"\nSTOCK ITEMS : "<<stockItems;

}


void MainWindow::on_optimizeButton_clicked()
{
if(!alreadyran){
alreadyran=true;
reset();
getTextData();

if(cutItems.isEmpty()||stockItems.isEmpty())
    {
    ui->textData->setText("<b> <font color=red>Enter all the values in the specified format.<font></b>");
    return;
    }
}
geneticParameter=ui->iterBox->value();
for(int i=1;i<=geneticParameter;i++)
{
//    std::random_shuffle(cutItems.begin(),cutItems.end());
    part=0;
    fullChromosome.clear();
    fullScrap=0;
//    fullMinScrap=0;
    minScrap=999999999;
    minTempStockItems=stockItems;
    while(true)
    {
        minScrap=999999999;

        tempCutItems=nextCutItems(&cutItems);
        currTempStockItems=minTempStockItems;
        if(tempCutItems.length()==0)
        {
            qDebug()<<"No parts remaining. means no items remaining to cut.";
            break;
        }

    minChromosome.clear();
    qDebug()<<"***Part"<<part;
//    qDebug()<<"Current Cut Items: "<<tempCutItems;
//    qDebug()<<"Stock Items : "<<minTempStockItems;

    qApp->processEvents();
    generation();
    ui->progressBar->setValue(i*100/geneticParameter);

    qDebug()<<minChromosome<<minChromosome.length();
    fullChromosome.append(minChromosome);
    fullScrap=minScrap;
    part++;
    ui->progressBar->setValue(part*100.0/(cutItems.length()/part));
    qApp->processEvents();

    }

    qDebug()<<fullScrap<<fullMinScrap;
    if(fullScrap<fullMinScrap)
    {
        fullMinScrap=fullScrap;
        fullMinChromosome=fullChromosome;
        qDebug()<<"Full Min Scrap: "<<fullMinScrap;
    }
    ui->scrapNumber->display(QString::number(fullMinScrap));
    qApp->processEvents();

}
qDebug()<<"\n[+]Full Chromosome"<<fullMinChromosome<<fullMinChromosome.length()<<minTempStockItems.length();

}

int MainWindow::generation()
{

for(int i=0;i<=10000;i++)
{ 
    tempStockItems=currTempStockItems;
    tempCutItems=nextCutItems(&cutItems);

    reset();
    stockMaxIndex=qrand()%tempStockItems.length();
    for(int i=0; i<tempCutItems.length();i++)
    {
        cutMaxIndex=qrand()%tempCutItems.length();

        while(tempCutItems[cutMaxIndex]==0)
        {
            cutMaxIndex=qrand()%tempCutItems.length();
        }

        int k=0;
if(qrand()%100<=ui->dataSlider->value())
        for(int p=0;p<tempStockItems.length();p++)
        {
            //qDebug()<<"[+++++++] Cut Item"<<tempCutItems[cutMaxIndex]<<"Stock item "<<tempStockItems[stockMaxIndex];
          stockMaxIndex=p;
          if(tempStockItems[stockMaxIndex]>=tempCutItems[cutMaxIndex])
              break;
        }
        else
        while(tempStockItems[stockMaxIndex]<tempCutItems[cutMaxIndex]&&k++<100)
        {
                //qDebug()<<"[+++++++] Cut Item"<<tempCutItems[cutMaxIndex]<<"Stock item "<<tempStockItems[stockMaxIndex];
              stockMaxIndex=qrand()%tempStockItems.length();
        }

        if(tempStockItems[stockMaxIndex]>=tempCutItems[cutMaxIndex]&&chromosome[cutMaxIndex]==-1)
        {
            chromosome[cutMaxIndex]=stockMaxIndex;
            tempStockItems[stockMaxIndex]-=tempCutItems[cutMaxIndex];
            tempCutItems[cutMaxIndex]=0;
//            qDebug()<<"Item cut"<<tempCutItems;
//            if(tempCutItems.count(0)==tempCutItems.length())
//                break;
        }
    }
//    qDebug()<<"part"<<part<<chromosome;
    if(chromosome.contains(-1))
        continue;
    for(int i=0;i<tempStockItems.length();i++)
    {
        if(tempStockItems[i]!=stockItems[i])
            scrap+=tempStockItems[i];
    }

    if(scrap<minScrap)
    {
        minScrap=scrap;
        minChromosome=chromosome;
        minTempStockItems=tempStockItems;
        qDebug()<<"********************************Got Minimum, Part "<<part;
    }
}

return 0;
}

void MainWindow::on_showResult_clicked()
{
    fitnessCalc();
}

void MainWindow::fitnessCalc()
{
    ui->outputTable->clearContents();
    ui->outputTable->setRowCount(0);
    ui->outputTable->setHorizontalHeaderLabels(QStringList()<<"Bar Length From Stock"<<"Cuts"<<"Length Used"<<"Scrap");

    QString output,excelData="Bar Length From Stock \t Cuts \t Length Used\t Scrap\n";
    QList<int> chr;
    QList<int> temp=fitness;
    qSort(temp);
    qSort(scraps);

    if(temp.isEmpty())
        return;

//    int minIndex=fitness.indexOf(temp.last());
//    chr=chromosomes.at(minIndex);
    chr=fullMinChromosome;

    int toCutFrom;
    QList<QStringList> result;
    for(int i=0;i<stockItems.length();i++)
            result.insert(i,QStringList(QString::number(stockItems.at(i))));


    for(int i=0;i<chr.length();i++){
        toCutFrom=chr[i];
        result[toCutFrom].append(QString::number(cutItems.at(i)));
//        qDebug()<<cutItems.at(i);

    }
//        qDebug()<<result<<scraps;
        long long materialused=0,totalScrap=0;
        foreach(QStringList res,result)
        {
            qDebug()<<res;
        if(res.length()>1)
        {
            int stockitem=res.first().toInt();
            long long used=0;
            foreach(QString cut,res.mid(1)){
                used+=cut.toInt();
            }
            materialused+=res.first().toInt();
            ui->outputTable->insertRow(0);
            ui->outputTable->setItem(0,0,new QTableWidgetItem(res.first()));
            ui->outputTable->setItem(0,1,new QTableWidgetItem(res.mid(1).join(',')));
            ui->outputTable->setItem(0,2,new QTableWidgetItem(QString::number(used)));
            ui->outputTable->setItem(0,3,new QTableWidgetItem(QString::number(stockitem-used)));
            excelData+=res.first()+"\t"+res.mid(1).join(' ')+"\t"+QString::number(used)+"\t"+QString::number(stockitem-used)+"\n";
            totalScrap+=stockitem-used;
            output+=res.first()+" : "+res.mid(1).join(",")+" "+QString::number(stockitem-used)+"\n";
        }
        }
        excelData+="Material Used\t"+QString::number(materialused)+"\t"+"Total Scrap \t"+QString::number(totalScrap);
        qApp->clipboard()->setText(excelData);
        ui->scrapNumber->display(QString::number(totalScrap));
        ui->materialUsed->setText(QString::number(materialused));
//        ui->textData->setText(output+"\n"+QString::number(scraps.first()));
}


void MainWindow::crossover()
{

}

void MainWindow::mutation()
{

}

void MainWindow::selection()
{
    int total=chromosomes.length(),toSelect=total/2;
qDebug()<<"Current Population: "<<total<<chromosomes<<fitness;

//Selection
chromosomes=chromosomes.mid(toSelect);
fitness=fitness.mid(toSelect);
total=chromosomes.length();
qDebug()<<"Selected Population: "<<total<<chromosomes<<fitness;

}




//void MainWindow::generation()
//{
//clear();
//scrap=0;
//tempStockItems=stockItems;
//tempCutItems=cutItems;
//for(int i=0; i<tempCutItems.length();i++)
//{
//    int cutMaxIndex=max(tempCutItems);
//    int stockMaxIndex=maxfit(tempStockItems,tempCutItems.at(cutMaxIndex));
//    if(tempStockItems[stockMaxIndex]>=tempCutItems[cutMaxIndex]&&chromosome.at(cutMaxIndex)==-1)
//    {
//        chromosome[cutMaxIndex]=stockMaxIndex;
//        tempStockItems[stockMaxIndex]-=tempCutItems[cutMaxIndex];
//        tempCutItems[cutMaxIndex]=0;
//    }
//}
//qDebug()<<"Chromosome :"<<chromosome;
//qDebug()<<"Orig. stock Items :"<<stockItems;
//qDebug()<<"Temp stock Items"<<tempStockItems;
//foreach(int a,tempStockItems)
//    scrap+=a;
//qDebug()<<"Scrap"<<scrap;
//}

//void MainWindow::on_selection_clicked()
//{
//    selection();
//}
//void MainWindow::on_showPopulation_clicked()
//{
//qDebug()<<"Total Chromosomes:"<<chromosomes.length()<<"Chromosomes: "<<chromosomes<<"\nFitness items: "<<fitness.length()<<"Fitness : "<<fitness;
//}

int MainWindow::minfit(QList<int> t,int cutting)
{
    QList<int> temp=t;
    qSort(temp);
    int minimumElement;
    qDebug()<<"Stock Items Min Fit: "<<t;
    while(!temp.isEmpty())
    {
        minimumElement=temp.takeFirst();
    if(cutting<=minimumElement)
    {
        qDebug()<<"Minimum element index"<<t.indexOf(minimumElement);
    return t.indexOf(minimumElement);
    }
    }

    return t.indexOf(minimumElement);

}

int MainWindow::maxfit(QList<int> t, int cutting)
{
    QList<int> temp=t;
    qSort(temp);
    int maximumElement;
    qDebug()<<"Stock Items Max Fit: "<<t;
    while(!temp.isEmpty())
    {
        maximumElement=temp.takeLast();
    if(cutting<=maximumElement)
    {
        qDebug()<<"Minimum element index"<<t.indexOf(maximumElement);
    return t.indexOf(maximumElement);
    }
    }

    return t.indexOf(maximumElement);
}

int MainWindow::max(QList<int> t)
{
    QList<int> temp=t;
    qSort(temp);
//    qDebug()<<"Cutting List "<<t<<"Maximum Elemen:"<<temp.last();
    return t.indexOf(temp.last());
}
int MainWindow::min(QList<int> t)
{
    QList<int> temp=t;
    temp.removeAll(0);
//    qSort(temp);

    if(temp.isEmpty()){
        return -1;
    }

    //    qDebug()<<"Cutting List "<<t<<"Minimum Elemen:"<<temp.first();
    int min=temp.first();
    int minindex=0;

    for(int i=0;i<temp.length();i++)
        if(temp[i]<min)
        {
            min=temp[i];
            minindex=i;
        }
    return t.indexOf(min);
//    return minindex;
}

void MainWindow::on_pushButton_clicked()
{
    alreadyran=false;
}
