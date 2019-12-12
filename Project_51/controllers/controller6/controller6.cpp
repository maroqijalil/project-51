#include <iostream>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Camera.hpp>
#include <webots/DistanceSensor.hpp>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <webots/Display.hpp>
#include <webots/ImageRef.hpp>

#define TIME_STEP 64
#define NB_FILTERS 6

using namespace cv;
using namespace webots;
using namespace std;

static unsigned char *hasil_img;
static unsigned char *hasil_img2;
static unsigned char *hasil_img3;
static int w;
static int h;


void gambar3(const unsigned char *image, int length) {
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);
  
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(20, 100, 100), Scalar(30, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        terfilt.at<Vec4b>(i, j) = img.at<Vec4b>(i, j);
      }
    }
  }

  memcpy(hasil_img3, terfilt.data, length);
  //cvtColor(hasil_img, hasil_img, COLOR_HSV2BGR);
}


void gambar2(const unsigned char *image, int length) {
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);
  
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(25, 52, 72), Scalar(102, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        terfilt.at<Vec4b>(i, j) = img.at<Vec4b>(i, j);
      }
    }
  }

  memcpy(hasil_img2, terfilt.data, length);
  //cvtColor(hasil_img, hasil_img, COLOR_HSV2BGR);
}

void gambar(const unsigned char *image, int length) {
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);
  
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(94, 80, 2), Scalar(126, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        terfilt.at<Vec4b>(i, j) = img.at<Vec4b>(i, j);
      }
    }
  }

  memcpy(hasil_img, terfilt.data, length);
  //cvtColor(hasil_img, hasil_img, COLOR_HSV2BGR);
}

int detect_bola(const unsigned char *image, int length) {  
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(94, 80, 2), Scalar(126, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        return 1;
      }
    }
  }
 
  return 0;
}


void posisi(const unsigned char *image, int length, int *kiri, int *kanan, int*bawah) {
  int penghitung, penghitung2, p, p2=0, pembanding, pembanding2=0;
  
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(94, 80, 2), Scalar(126, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    penghitung=0; penghitung2=0; p=0;
    for (int j = 0; j < w; ++j) {
      if (p==0) penghitung2++;
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        penghitung++; p=1;
        *bawah=i;
      }
    }
    pembanding=w-(penghitung2+penghitung);
    //if(p==1) p2++;
    if(pembanding>pembanding2){
      pembanding2=pembanding;
      *kiri=penghitung2;
      *kanan=w-(penghitung2+penghitung);
    }
  }
  
  return;
}


void posisi_gawang(const unsigned char *image, int length, int *kiri, int *kanan, int *kiri2, int *kanan2) {
  int penghitung, penghitung2, p, p2=0, pembanding, pembanding2=0, p_t;
  *kiri=0; *kanan=0; *kiri2=0; *kanan2=0;
  
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(20, 100, 100), Scalar(30, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    // cout << i << ".. " << endl;
    penghitung=0; penghitung2=0; p=0; p_t=0;
    for (int j = 0; j < w/2; ++j) {
      // if (p==0) penghitung2++;
      if (sementara_terfilt.at<uchar>(i, j) != 255 && p==0) {
        penghitung=j;
      } else if(sementara_terfilt.at<uchar>(i, j) == 255) p=1;
      if (sementara_terfilt.at<uchar>(i, (w-1)-j) != 255 && p_t==0){
        penghitung2=j;
      } else if(sementara_terfilt.at<uchar>(i, (w-1)-j) == 255) p_t=1;
    }
    if((p==1&&p_t==1)&&(penghitung!=0||penghitung2!=0)){
      *kiri=penghitung;
      *kanan=penghitung2;
    }
    if(penghitung!=0||penghitung2!=0){
      if (p==1) *kiri2=penghitung;
      if (p_t==1) *kanan2=penghitung2;
    }
  }
  
  return;
}


int detect_dinding(const unsigned char *image, int length) {  
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(25, 52, 72), Scalar(102, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        return 1;
      }
    }
  }
 
  return 0;
}


int detect_gawang(const unsigned char *image, int length) {  
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);

  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      terfilt.at<Vec4b>(i, j)[0] = 0;
      terfilt.at<Vec4b>(i, j)[1] = 0;
      terfilt.at<Vec4b>(i, j)[2] = 0;
      terfilt.at<Vec4b>(i, j)[3] = 255;
    }
  }

  inRange(hsv, Scalar(20, 100, 100), Scalar(30, 255, 255), sementara_terfilt);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (sementara_terfilt.at<uchar>(i, j) == 255) {
        return 1;
      }
    }
  }
 
  return 0;
}


int main(int argc, char **argv) {
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  Motor *wheels[4], *capitm[2];
  Motor *sodoker;
  Camera *cam;
  Display *disp;
  Display *disp2;
  Display *disp3;
  ImageRef *img_ref;
  ImageRef *img_ref2;
  ImageRef *img_ref3;
  Motor *sodoker2;
  Motor *angkat;
  Motor *dep_capitm[2];
  Motor *pelm[2];
  
  double p_a=0, p_b=0;
  int penanda_putar=0;
  int putar=0;
  int ukuran;
  int jalan=0, penanda_jalan=0;
  int p_kiri, p_kanan, p_bawah, p_kiri_g, p_kanan_g, p_kiri_g2, p_kanan_g2;
  int penanda_main=0, ppm=0;
  double ban_kiri, ban_kanan;
  int penanda_bola;
  char dsNames[3][10] = {"ds_right", "ds_left", "ds_top"};
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  char capit[2][8] = {"capit_a", "capit_b"};
  char dep_capit[2][10] = {"dep_c_a", "dep_c_b"};
  char pel[2][8] = {"pel_a", "pel_b"};
  int avoidObstacleCounter = 0;
  const unsigned char *img;
  double leftSpeed;
  double rightSpeed;
  int penanda_pel=0;
  
  sodoker = robot->getMotor("sodok");
  cam = robot->getCamera("camera");
  sodoker2 = robot->getMotor("angkat_s");
  angkat = robot->getMotor("angkat_a");
  
  for (int i = 0; i < 3; i++) {
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);
  }
  
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
    if(i<=1){
      capitm[i] = robot->getMotor(capit[i]);
      capitm[i]->setPosition(p_b);
      dep_capitm[i] = robot->getMotor(dep_capit[i]);
      dep_capitm[i]->setPosition(p_b);
      pelm[i] = robot->getMotor(pel[i]);
      pelm[i]->setPosition(p_b);
    }
  }
  sodoker->setPosition(p_a);
  
  cam->enable(TIME_STEP);
  w = cam->getWidth();
  h = cam->getHeight();
  
  disp = robot->getDisplay("display");
  disp2 = robot->getDisplay("display1");
  disp3 = robot->getDisplay("display2");
  ukuran = 4*w*h*sizeof(unsigned char);
  hasil_img = (unsigned char *)malloc(ukuran);
  hasil_img2 = (unsigned char *)malloc(ukuran);
  hasil_img3 = (unsigned char *)malloc(ukuran);
  
  ban_kiri = 2.0;
  ban_kanan = -2.0;
  
  while (robot->step(TIME_STEP) != -1) {
    img = cam->getImage();
    p_bawah=0;
    posisi(img, ukuran, &p_kiri, &p_kanan, &p_bawah);
    
    if(penanda_main==0){
    
      if(penanda_pel==0){
        pelm[0]->setPosition(p_b+0.03);
        pelm[1]->setPosition(p_b+0.03);
      }
      
      leftSpeed = 3.0;
      rightSpeed = 3.0;
      
      if(putar>=110){
        jalan = 100;
        penanda_putar=1;
        putar=0;
      }
      
      if(jalan>0&&penanda_jalan==0){
        jalan--;
        if(jalan==0){
          penanda_putar=0;
        }
      }
      
      if (avoidObstacleCounter > 0) {
        avoidObstacleCounter--;
      } else {
        for (int i = 0; i < 3; i++) {
          if (ds[i]->getValue() < 1000.0){
            if(ds[0]->getValue() < 1000.0){
              leftSpeed = -2.0;
              rightSpeed = 2.0;
              ban_kiri = -2.0;
              ban_kanan = 2.0;
            } else if(ds[1]->getValue() < 1000.0){
              leftSpeed = 2.0;
              rightSpeed = -2.0;
              ban_kiri = 2.0;
              ban_kanan = -2.0;
            } else if(ds[2]->getValue() < 1000.0){
              leftSpeed = ban_kiri;
              rightSpeed = ban_kanan;
            }
            penanda_putar=1;
            penanda_jalan=1;
            //jalan =1;
          } else {
            penanda_jalan=0;
            penanda_putar=0;
          } 
          if(i<=1){
            dep_capitm[i]->setPosition(p_b);
            capitm[i]->setPosition(p_b);
          }
        }
      }
      
      if(detect_bola(img, ukuran)){
        sodoker2->setPosition(p_a);
        angkat->setPosition(p_a);
        if(h-p_bawah<=2&&p_bawah!=0){
          pelm[0]->setPosition(p_b+0.015);
          pelm[1]->setPosition(p_b+0.015);
          penanda_pel=1;
        } else {
          pelm[0]->setPosition(p_b+0.03);
          pelm[1]->setPosition(p_b+0.03);
        }
        if(p_kiri>p_kanan){
          leftSpeed = 2.0;
          rightSpeed = -2.0;
          ban_kiri = 2.0;
          ban_kanan = -2.0;
          putar=0;
        } else if(p_kiri<p_kanan){
          leftSpeed = -2.0;
          rightSpeed = 2.0;
          ban_kiri = -2.0;
          ban_kanan = 2.0;
          putar=0;
        }
        if((p_kiri-p_kanan)>=-15&&(p_kiri-p_kanan)<=15){
          leftSpeed = 3.0;
          rightSpeed = 3.0;
          penanda_bola = p_kiri;
          if(p_kiri<p_kanan) penanda_bola = p_kanan;
          cout << p_kiri << " " << p_bawah << " " << p_kanan << endl;
          cout << ds[2]->getValue() << endl;
          if (ds[2]->getValue()< 500.0&&p_bawah==h-1/*(w-(2*p_kiri+p_kanan))>-penanda_bola*/){
            cout << "asdas" << endl;
            leftSpeed = 0.0;
            rightSpeed = 0.0;
            penanda_main=1;
          }
        }
        penanda_putar = 1;
      } else if (penanda_putar==0&&jalan==0){
        pelm[0]->setPosition(p_b+0.03);
        pelm[1]->setPosition(p_b+0.03);
        leftSpeed = ban_kiri;
        rightSpeed = ban_kanan;
        putar++;
        cout << putar << endl;
      }
      cout << jalan << endl;

    } else if(penanda_main==1){
      leftSpeed = 0.0;
      rightSpeed = 0.0;
      posisi(img, ukuran, &p_kiri, &p_kanan, &p_bawah);
      pelm[0]->setPosition(p_b);
      pelm[1]->setPosition(p_b);
      
      capitm[0]->setPosition(p_b+0.03);
      capitm[1]->setPosition(p_b+0.03);
      dep_capitm[0]->setPosition(p_b+0.02);
      dep_capitm[1]->setPosition(p_b-0.02);
      cout << p_kiri << " " << p_kanan << endl;
      
      sodoker2->setPosition(p_a+0.03);
      angkat->setPosition(p_a-0.55);
      penanda_main = 2;
    
    } else if(penanda_main==2){
    
      if(p_bawah!=h-1||p_bawah==0){
        sodoker2->setPosition(p_a);
        angkat->setPosition(p_a);
        penanda_pel=0;
        penanda_main=0;
      } else {
        p_kiri_g=0; p_kanan_g=0;
        posisi_gawang(img, ukuran, &p_kiri_g, &p_kanan_g, &p_kiri_g2, &p_kanan_g2);
        
        leftSpeed = 3.0;
        rightSpeed = 3.0;
        
        if(putar>=110){
          jalan = 100;
          penanda_putar=1;
          putar=0;
        }
        
        if(jalan>0&&penanda_jalan==0){
          jalan--;
          if(jalan==0){
            penanda_putar=0;
          }
        }
        
        if (avoidObstacleCounter > 0) {
          avoidObstacleCounter--;
        } else {
          for (int i = 0; i < 3; i++) {
            if (ds[i]->getValue() < 1000.0){
              if(ds[0]->getValue() < 1000.0){
                leftSpeed = -2.0;
                rightSpeed = 2.0;
                ban_kiri = -2.0;
                ban_kanan = 2.0;
              } else if(ds[1]->getValue() < 1000.0){
                leftSpeed = 2.0;
                rightSpeed = -2.0;
                ban_kiri = 2.0;
                ban_kanan = -2.0;
              } else if(ds[2]->getValue() < 1000.0){
                leftSpeed = ban_kiri;
                rightSpeed = ban_kanan;
              }
              penanda_putar=1;
              penanda_jalan=1;
              //jalan =1;
            } else {
              penanda_jalan=0;
              penanda_putar=0;
            }
          }
        }
        
        if(p_kiri_g!=0 && p_kanan_g!=0){
          if(p_kiri_g-p_kanan_g<=2 && p_kiri_g-p_kanan_g>=-2){
            penanda_main = 3;
          } else if(p_kiri_g2<p_kanan_g2){
            leftSpeed = -1.0;
            rightSpeed = 1.0;
            ban_kiri=-2.0;
            ban_kanan=2.0;
          } else if(p_kiri_g2>p_kanan_g2){
            leftSpeed = 1.0;
            rightSpeed = -1.0;
            ban_kiri=2.0;
            ban_kanan=-2.0;
          }
          penanda_putar = 1;
        } else if (penanda_putar==0&&jalan==0){
          leftSpeed = ban_kiri;
          rightSpeed = ban_kanan;
          putar++;
          cout << putar << endl;
        }
        cout << p_kiri_g2 << " " << p_kanan_g2 << endl;
        
      }
    
    } else if (penanda_main==3){
      leftSpeed = 0.0;
      rightSpeed = 0.0;
      
      dep_capitm[0]->setPosition(p_b);
      dep_capitm[1]->setPosition(p_b);
      capitm[0]->setPosition(p_b);
      capitm[1]->setPosition(p_b);
      sodoker->setPosition(p_a-0.02);
      
      sodoker->setAcceleration(15.0);
      sodoker->setPosition(p_a+0.02);
      
      if(p_bawah!=h-1||p_bawah==0){
        sodoker->setPosition(p_a);
        sodoker2->setPosition(p_a);
        angkat->setPosition(p_a);
        penanda_pel=0;
        penanda_main=0;
      }
    }
    
    wheels[0]->setVelocity(leftSpeed);
    wheels[1]->setVelocity(rightSpeed);
    wheels[2]->setVelocity(leftSpeed);
    wheels[3]->setVelocity(rightSpeed);
    
    gambar(img, ukuran);
    gambar2(img, ukuran);
    gambar3(img, ukuran);
    
    img_ref = disp->imageNew(w, h, hasil_img, 4);
    disp->imagePaste(img_ref, 0, 0, false);
    img_ref2 = disp2->imageNew(w, h, hasil_img2, 4);
    disp2->imagePaste(img_ref2, 0, 0, false);
    img_ref3 = disp3->imageNew(w, h, hasil_img3, 4);
    disp3->imagePaste(img_ref3, 0, 0, false);
  }
  
  disp->imageDelete(img_ref);
  free(hasil_img);
  disp2->imageDelete(img_ref2);
  free(hasil_img2);
  disp3->imageDelete(img_ref3);
  free(hasil_img3);
  cam->disable();
  
  delete robot;
  return 0;
}







// int main() {
  // /* Initialize Webots */
  // wb_robot_init();
  // int timestep = wb_robot_get_basic_time_step();

  // printf("Vision module demo, using openCV.\n");

  // display_commands();

  // /* Initialize camera */
  // WbDeviceTag camera = wb_robot_get_device("camera");
  // wb_camera_enable(camera, timestep);
  // width = wb_camera_get_width(camera);
  // height = wb_camera_get_height(camera);

  // /* Variables for the display */
  // int length = 4 * width * height * sizeof(unsigned char);
  // WbDeviceTag processed_image_display = wb_robot_get_device("proc_im_display");
  // WbImageRef processed_image_ref = NULL;
  // processed_image = (unsigned char *)malloc(length);

  // wb_keyboard_enable(timestep);

  // int input = 0;
  // bool key_pressed = false;

  // while (wb_robot_step(timestep) != -1) {
    // /* Process inputs */
    // const int key = wb_keyboard_get_key();
    // if (key >= 0 && !key_pressed) {
      // key_pressed = true;
      // input = key;
    // } else if (key == -1 && key_pressed) {
      // key_pressed = false;
      // switch (input) {
        // case 'X':
          // apply_filter(NONE);
          // break;
        // case 'R':
          // apply_filter(RED);
          // break;
        // case 'G':
          // apply_filter(GREEN);
          // break;
        // case 'B':
          // apply_filter(BLUE);
          // break;
        // case 'Y':
          // apply_filter(YELLOW);
          // break;
        // case 'P':
          // apply_filter(PURPLE);
          // break;
        // case 'W':
          // apply_filter(WHITE);
          // break;
        // case 'A':
          // apply_filter(ALL);
          // break;
        // default:
          // break;
      // }
    // }

    // /* Process the image */
    // process_image(wb_camera_get_image(camera), length);

    // if (processed_image_ref) {
      // wb_display_image_delete(processed_image_display, processed_image_ref);
      // processed_image_ref = NULL;
    // }

    // /* Display the image */
    // processed_image_ref = wb_display_image_new(processed_image_display, width, height, processed_image, WB_IMAGE_BGRA);
    // wb_display_image_paste(processed_image_display, processed_image_ref, 0, 0, false);
  // }

  // //clean up
  // if (processed_image_ref)
    // wb_display_image_delete(processed_image_display, processed_image_ref);
  // free(processed_image);

  // wb_robot_cleanup();

  // return 0;
// }
