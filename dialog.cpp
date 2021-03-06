#include "dialog.h"
#include "ui_dialog.h"
#include <Qtcore>
/////////////////////////////////////////////////////////////////////////////////////////////
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    filerecord = "ALPRS_RECORDS.txt";
motioncount =0; picount=0; stringcount=0;
cpt=0;

}
///////////////////////////////////////////////////////////////////////////////////////////////
Dialog::~Dialog()
{
    delete ui;
}
//////////////////////////////////////////////////////

bool sortvector(std::vector<Point> first, std::vector<Point> second) {

return first[0].x < second[0].x;
}
//////////////////////////////////////////////////////////////////////////

char * remove_spaces(char * source, char * target)
{
     while(*source++ && *target)
     {
        if (!isspace(*source))
             *target++ = *source;
     }
     return target;
}

/////////////////////////////////////////////////////////////////////////////

void Dialog::processFrameAndUpdateGUI(){

    bool sortvector(std::vector<Point> first, std::vector<Point> second);
        Rect trigger(780, 150, 20, 60);
       CvScalar black = CV_RGB( 0, 0, 0 );
        vector<vector<Point> > contours;
         vector<Vec4i> hierarchy;
         QString LP[33] = {"WSU5746","MBX3681","MAU6689","WDB6517","PKL6682","MAK5387","WSS367","MCC8294","MBS5750","WSB3584"
                           ,"MBJ2882","WFA4987","MCE9938","WEP2399","JFL4544","MBR3886","MCC8836","WQW7083","MCD7136","MCB81",
                           "MBY3614","MU6163","MBU3939","JJW3672","MCB3136","JES2060","MBN6118","MBC153","WAT8816","MAQ6713",
                           "MBR8617","MBM8453","WLA9368"};



         input.read(frame2);
                  if(frame2.empty())  // check if we succeeded
                    {
                      ui->Voutput->clear();
                      ui->plainTextEdit->clear();
                      ui->stop->setDisabled(TRUE);
                      ui->btnResume->setDisabled(TRUE);
                      ui->btnPause->setDisabled(TRUE);
                      ui->loadfile->setEnabled(TRUE);
                      ui->loadcamera->setEnabled(TRUE);
                      ui->viewreport->setEnabled(TRUE);
                      return;
                    }

                     img = frame2(Rect(500, 100, 300, 150));
                                               draw = img;
                                               imROI2 = frame2(trigger);
                                              cvtColor( imROI2, imggray2, CV_RGB2GRAY );
                                              absdiff(imggray1,imggray2,imggray);
                                             cv::threshold (imggray, imggray, 70, 255, THRESH_BINARY);
                                             Pixels  = countNonZero(imggray);

                                         if (Pixels < 4)
                                              flag =0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                         if (( Pixels  >= 4 )&&(flag ==0 )) {
                                             flag = 1; motioncount++;
                                            cout<<"Motion Detected = "<<motioncount<<endl;

                                             ui->IPoutput->clear();
                                             ui->txtcharoutput->clear();

                                             cvtColor( img, img, CV_RGB2GRAY );
                                           cv::threshold (img, img, 230, 255, THRESH_BINARY);
                                             Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 5));
                                             dilate (img,img,element);
                                             erode (img,img,Mat());
                                             erode (img,img,Mat());
                                             erode (img,img,Mat());


                                            l=0;
                                             findContours( img, contours,hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );

                                            for( int i = 0; i < contours.size(); i++ )
                                                 {
                                                    if (( contourArea (contours[i]) < 4500) && (contourArea (contours[i]) > 1000))
                                                    {

                                                       bound = boundingRect (contours[i]);
                                                        img = draw(bound);

                                                        tresh = img;
                                                                                                                 cpt=0;l=0;

                                                                                                                 cvtColor( tresh, tresh, CV_RGB2GRAY );
                                                                                                                     cv::threshold (tresh, tresh, 230, 255, THRESH_BINARY);
                                                                                                                    // imshow("platetresh", tresh);
                                                                                                                     findContours( tresh, contours2,hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
                                                                                                                     for (int i=0; i<contours2.size(); i++)
                                                                                                                     if(( contourArea (contours2[i]) < 300) && (contourArea (contours2[i]) > 10))
                                                                                                                     cpt++;
                                                                                                                     if (cpt++>=4){

                                                                                                                         l=1;
                                                                                                                         QImage PImage ((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
                                                                                                                      ui->IPoutput->setPixmap(QPixmap::fromImage(PImage));
                                                                                                                         break;
                                                                                                                     }

                                                    }

                                                 }

                                            if(l==1)

                                            {
                                                timediff.start();

                                                copyMakeBorder(img, img,2 ,2 ,2 ,2 , BORDER_REPLICATE);

                                             draw = img;
                                                 k = 0;
                                                cvtColor( img, img, CV_RGB2GRAY );
                                                     cv::threshold (img, img, 220, 255, THRESH_BINARY);

                                                 findContours( img, contours1,hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );


                                                 sort(contours1.begin(), contours1.end(), sortvector);

                                                 for( int i = 0; i < contours1.size(); i++ )
                                                  {

                                                         bound = boundingRect (contours1[i]);
                                                         images[i] = draw(bound);

                                                         imwrite("images[i].jpg", images[i]);
                                                         images[i] = imread ("images[i].jpg");

                                                         copyMakeBorder(images[i], images[i],4 ,4 ,4 ,4 , BORDER_CONSTANT, Scalar::all(0) );
                                                         double area;

                                                         bcg = images[i];
                                                         cvtColor( images[i], images[i], CV_RGB2GRAY );
                                                         cv::threshold (images[i], images[i], 200, 255, THRESH_BINARY);

                                                 findContours( images[i], contours2,hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

                                                 if (contours2.size()>1)
                                                 for( int j = 0; j < contours2.size(); j++ )
                                                 {

                                                     area = contourArea(contours2[j]);

                                                     if(  area < 30 )  // if the area of the contour is less than threshold remove it
                                                     {

                                                         drawContours( bcg, contours2,j, black,-1,1,hierarchy ); // removes white dots

                                                     }
                                                 }

                                                         images[i] = bcg;
                                                         k = k+1;

                                                  }

                                                 for (int i=0; i<k; i++)
                                                 {
                                                     size=0; test = images[i];
                                                     cvtColor( test, test, CV_RGB2GRAY );
                                                   // pyrUp( test, test, Size( test.cols*2, test.rows*2 ) );
                                                    // pyrUp( test, test, Size( test.cols*2, test.rows*2 ) );


                                                     cv::threshold (test, test, 215, 255, THRESH_BINARY);


                                                tesseract::TessBaseAPI tess;
                                                 tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
                                                 tess.SetVariable("tessedit_char_whitelist", "QWERTYUPASDFGJKLZXCVBNM0123456789");

                                                 tess.SetImage((uchar*)test.data, test.size().width, test.size().height, test.channels(), test.step1());
                                                 tess.Recognize(0);
                                                 c += tess.GetUTF8Text();

                                              //  ui->txtcharoutput->appendPlainText(out);
                                                    }

                                                 c.replace("\n","");
                                                 //c.replace(""," ");

                                                if(c.length()>=5){
                                                     picount++;
                                                     cout <<"Image Plate detected = "<< picount << endl;}

                                                 for (int i=0; i<33; i++)

                                                     if (LP[i]==c){
                                                         stringcount++;
                                                         cout <<"Correct Plate Detected = "<< stringcount << endl;
                                                         break;}





                                                 dateTime = QDateTime::currentDateTime();
                                                 dateTimeString = dateTime.toString();
                                                 ui->txtcharoutput->setText(c);

                                                 timeresult = timediff.elapsed();
                                                 cout <<"recognition time = " << timeresult << endl;


                                               ui->plainTextEdit->appendPlainText(dateTimeString + "   :   " + c);


                                                             QFile file(filerecord);
                                                             if (!file.open(QIODevice::Append)) {
                                                                 QMessageBox::critical(this, tr("Error"), tr("The Record File Could Not Open successfully !"));
                                                             } else {
                                                                 QTextStream stream(&file);
                                                                stream << dateTimeString + "   :   " + c +"\n";
                                                                 stream.flush();
                                                                 file.close();
                                                             }

                                                              c="";



                                                 }





                                    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                                             cv::cvtColor(frame2,frame2,CV_BGR2RGB);
                                              QImage Video ((uchar*)frame2.data, frame2.cols, frame2.rows, frame2.step, QImage::Format_RGB888);
                                               ui->Voutput->setPixmap(QPixmap::fromImage(Video));


                                               //get current date
                                                   date = QDate::currentDate();
                                                 QString dateString = date.toString();
                                                   ui->outputdate->setText(dateString);

                                                   //get current time
                                                  time = QTime::currentTime();
                                                   QString timeString = time.toString();
                                                   ui->outputtime->setText(timeString);




        }





////////////////////////////////////////////////////////////////////////////////////////////////


void Dialog::on_loadfile_clicked()
{
    tmrTimer = new QTimer(this);
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open Video File"), "", tr("Video Files (*.AVI *.WMV)"));
        QByteArray ba = filePath.toLocal8Bit();
        QFileInfo file(filePath);
            //QString fileSuffix = file.();


            fileName = ba.data();
            input.open(fileName);

           if(input.isOpened()== false)  // check if we succeeded
           {
           //    ui->txtcharoutput->appendPlainText("Error: Video File Not Loaded !");

                QMessageBox::critical(this, tr("Error"), tr("The Video File is Not Loaded Successfully !"));

                return;
           }

           input.read(frame1);
               if(frame1.empty())  // check if we succeeded
              return;
               if ((frame1.cols !=800)&&(frame1.rows!=600)){

                   QMessageBox::critical(this, tr("Error"), tr("The Video File Resolution Doesn't Match !"));

                   return;

               }

                  ui->loadfile->setDisabled(TRUE);
                  ui->stop->setEnabled(TRUE);
                  ui->loadcamera->setDisabled(TRUE);
                  ui->btnPause->setEnabled(TRUE);
                  ui->viewreport->setDisabled(TRUE);
                  ui->plainTextEdit->clear();

                  QFile file1(filerecord);
                  if (!file1.open(QIODevice::Append)) {
                      QMessageBox::critical(this, tr("Error"), tr("The Record File Could Not Open successfully !"));
                  } else {
                      QTextStream stream(&file1);
                      date = QDate::currentDate();
                      QString dateString = date.toString();
                      stream << "\n//////////////////  "+dateString +"  /////////////////\n";
                      stream.flush();
                      file1.close();
                  }


              imROI1 = frame1(Rect(780, 150, 20, 60));
              cvtColor( imROI1, imggray1, CV_RGB2GRAY );
              threshold = (double)(imROI1.rows * imROI1.cols) * 0.7;


           connect (tmrTimer, SIGNAL (timeout()), this, SLOT(processFrameAndUpdateGUI()));
           tmrTimer->start(50);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dialog::on_stop_clicked()
{
    destroyed(tmrTimer);
       motioncount =0; picount=0; stringcount=0;

       input.release();
       ui->stop->setDisabled(TRUE);
       ui->btnResume->setDisabled(TRUE);
       ui->btnPause->setDisabled(TRUE);
       ui->loadfile->setEnabled(TRUE);
       ui->loadcamera->setEnabled(TRUE);
       ui->viewreport->setEnabled(TRUE);
       ui->plainTextEdit->clear();
       tmrTimer->stop();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dialog::on_loadcamera_clicked()
{
    tmrTimer = new QTimer(this);
        input.open(0);
        input.set(CV_CAP_PROP_FRAME_WIDTH,800);
        input.set(CV_CAP_PROP_FRAME_HEIGHT,600);

       if(input.isOpened()== false)  // check if we succeeded
       {
       //    ui->txtcharoutput->appendPlainText("Error: Video File Not Loaded !");

            QMessageBox::critical(this, tr("Error"), tr("The Camera is Not connected Successfully !"));

            return;
       }

       input.read(frame1);
           if(frame1.empty())  // check if we succeeded
          return;


        ui->loadfile->setDisabled(TRUE);
        ui->stop->setEnabled(TRUE);
           ui->loadcamera->setDisabled(TRUE);
           ui->btnPause->setEnabled(TRUE);
           ui->viewreport->setDisabled(TRUE);
           ui->plainTextEdit->clear();

           imROI1 = frame1(Rect(780, 150, 20, 60));
           cvtColor( imROI1, imggray1, CV_RGB2GRAY );
           //threshold = (double)(imROI1.rows * imROI1.cols) * 0.7;

       connect (tmrTimer, SIGNAL (timeout()), this, SLOT(processFrameAndUpdateGUI()));
       tmrTimer->start(50);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dialog::on_btnResume_clicked()
{
    ui->btnPause->setEnabled(TRUE);
        ui->btnResume->setDisabled(TRUE);
        tmrTimer->start(50);

}

void Dialog::on_btnPause_clicked()
{
    ui->btnPause->setDisabled(TRUE);
       ui->btnResume->setEnabled(TRUE);
        tmrTimer->stop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dialog::on_exit_clicked()
{
    input.release();
        exit(0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dialog::on_viewreport_clicked()
{
    myreport = new report(this);

    myreport->show();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
