#include "./include/sequence.h"
#include <QWidget>
#include <QFileDialog>
#include <QtDebug>
#include <sstream>
#include <iostream>
#include <QVector>




// code modified from http://www.codebind.com/cpp-tutorial/qt-tutorial/qt-tutorials-beginners-qfiledialog-getopenfilename-example/
class QFileDialogTester : public QWidget
{
public:
  QString openFile()
  {
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    if( !filename.isNull() )
    {
     return filename;
    }
    return NULL;
  }
};

Sequence::Sequence() {
    ROWS =0;
    COLS =0;
    sequenceName ="";
}

Sequence::Sequence(int rows, int cols, int dur, std::string name) {
    ROWS =rows;
    COLS =cols;
    sequenceName = name;
    frame_dur = dur;
}

std::string Sequence::getName() {
    return sequenceName;
}

int Sequence::getRows() {
    return ROWS;
}

int Sequence::getCols() {
    return COLS;
}

int Sequence::getFrameDur() {
    return frame_dur;
}

QVector<QVector<int> > Sequence::getGrid() {
    return gridData;
}

void Sequence::save() {
    std::string frame_dur = std::to_string(getFrameDur());
    std::string cols = std::to_string(getCols());
    std::string rows = std::to_string(getRows());

    std::ofstream file (getName() + ".json");
     file << "{\"name\":\"" <<getName()<<"\",\"music_file\":\"" <<getName() <<".ogg\",\"frame_dur_ms\":"
            <<frame_dur <<"\",\"num_frames\":" << cols << ",\"rows\":" <<rows<< ",\"data\":" << "\n" << "[";

     QVector< QVector< int > > gridsave= gridData;
     for(int i =0; i<getCols();i++){
         file << "[";
             for (int j = 0; j<getRows();j++ ){
                 if( i == getCols()-1 && j== getRows()-1){
                     file << std::to_string(gridsave[i][j]) << "]";
                 }
                 else if (j == getRows()-1){
                     file << std::to_string(gridsave[i][j]) << "],";
                     }

                 else{
                     file <<std::to_string(gridsave[i][j])<<",";
                 }
             }

}
     file << "]";

 }

void Sequence::load() {
    QFileDialogTester test;
    QString filePath = test.openFile();
    std::string fileString = filePath.toUtf8().constData();
    std::ifstream File(fileString.c_str());
    std::string firstline;
    std::string secondline;
    std::string colsub;
    std::string col;
    std::string rowsub;
    std::string row;
    std::string num_frames = "num_frames\":" ;
    std::string rows = ",\"rows\":" ;
    std::string data = ",\"data\":";
    File >> firstline;
    File >> secondline;
    std::string thirdline = secondline;
    std::cout <<thirdline<< "here" <<std::endl;

    std::size_t found = firstline.find(num_frames);
    if (found!=std::string::npos){
        colsub = firstline.substr(found,firstline.length());
    }
    std::size_t found2 = colsub.find(rows);
    if (found2!=std::string::npos){
        col = colsub.substr(num_frames.length(),found2-rows.length()-4);
    }

    std::size_t found3 = firstline.find(rows);
    if (found3!=std::string::npos){
        rowsub = firstline.substr(found3,firstline.length());
    }
    std::size_t found4 = rowsub.find(data);
    if (found4!=std::string::npos){
        row = rowsub.substr(rows.length(),found4-data.length()-5);
    }

    int cols = std::stoi(col);
    int rowss = std::stoi(row);
    std::stringstream ss(secondline);
    int temp =0;
    QVector< QVector< int > > gridLoad;
    int count =0;
    int count2 =0;
    for(int i =0; i<cols;i++){
        QVector<int> column;
        gridLoad.push_back(column);

    }
    QVector< QVector< int > > gridCurrent = gridData;

        for( int i = 0; i<secondline.length(); i++){
        if((secondline[i] >= '0') && (secondline[i] <= '9')){
            int accum = i+1;
            std::string accums = "";
                    accums += secondline[i];
            while(secondline[accum] != ']'  && secondline[accum] != ',' ){
                accums += secondline[accum];
                accum++;
            }
                 int stringInt = std::stoi(accums);
                 gridLoad[count].push_back(stringInt);
                   count2++;
                i = i+accums.length();
                }

        if(count2 == rowss){
            count ++;
            count2 =0;
        }
}


        gridData = gridLoad;

}
