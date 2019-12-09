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
static int w;
static int h;

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

int process_image(const unsigned char *image, int length) {  
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


void posisi(const unsigned char *image, int length, int *kiri, int *kanan) {
  int penghitung, penghitung2, p, pembanding;
  
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
      }
    }
    pembanding=w-(2*penghitung2+penghitung);
    *kiri=penghitung2;
    *kanan=w-(penghitung2+penghitung);
    if(p==1&&penghitung>3){
      break;
    }
  }
  
  return;
}


int main(int argc, char **argv) {
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  Motor *wheels[4], *capitm[2];
  Motor *sodoker;
  Camera *cam;
  Display *disp;
  ImageRef *img_ref;
  Motor *sodoker2;
  Motor *angkat;
  
  double p_a=0, p_b=0;
  int penanda_putar=0;
  int putar=0;
  int ukuran;
  int jalan=0, penanda_jalan=0;
  int p_kiri, p_kanan;
  double ban_kiri, ban_kanan;
  char dsNames[3][10] = {"ds_right", "ds_left", "ds_top"};
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  char capit[2][8] = {"capit_a", "capit_b"};
  int avoidObstacleCounter = 0;
  const unsigned char *img;
  const unsigned char *img2;
  
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
    }
  }
  sodoker->setPosition(p_a);
  sodoker->setPosition(p_a+0.02);
  
  cam->enable(TIME_STEP);
  w = cam->getWidth();
  h = cam->getHeight();
  
  disp = robot->getDisplay("display");
  ukuran = 4*w*h*sizeof(unsigned char);
  hasil_img = (unsigned char *)malloc(ukuran);
  
  ban_kiri = 2.0;
  ban_kanan = -2.0;
  
  while (robot->step(TIME_STEP) != -1) {
    img = cam->getImage();
    double leftSpeed = 3.0;
    double rightSpeed = 3.0;
    
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
      capitm[0]->setPosition(p_b+1);
      capitm[1]->setPosition(p_b-1);
    } else {
      for (int i = 0; i < 3; i++) {
        if (ds[i]->getValue() < 1000.0){
          leftSpeed = 2.0;
          rightSpeed = -2.0;
          penanda_putar=1;
          penanda_jalan=1;
          //jalan =1;
        } else {
          penanda_jalan=0;
          penanda_putar=0;
        } 
        if(i<=1)
          capitm[i]->setPosition(p_b);
      }
    }
    
    if(process_image(img, ukuran)){    
      posisi(img, ukuran, &p_kiri, &p_kanan);
      if(p_kiri>p_kanan){
        leftSpeed = 2.0;
        rightSpeed = -2.0;
        ban_kiri = 2.0;
        ban_kanan = -2.0;
      } else if(p_kiri<p_kanan){
        leftSpeed = -2.0;
        rightSpeed = 2.0;
        ban_kiri = -2.0;
        ban_kanan = 2.0;
      }
      if((p_kiri-p_kanan)>=-10&&(p_kiri-p_kanan)<=10){
        leftSpeed = 3.0;
        rightSpeed = 3.0;
        if (ds[2]->getValue() < 500.0){
          leftSpeed = 0.0;
          rightSpeed = 0.0;
        }
      }
      penanda_putar = 1;
    }else if(penanda_putar==0&&jalan==0){
      leftSpeed = ban_kiri;
      rightSpeed = ban_kanan;
      putar++;
      cout << putar << endl;
    }
    cout << jalan << endl;
    
    wheels[0]->setVelocity(leftSpeed);
    wheels[1]->setVelocity(rightSpeed);
    wheels[2]->setVelocity(leftSpeed);
    wheels[3]->setVelocity(rightSpeed);
    
    gambar(img, ukuran);
    
    img_ref = disp->imageNew(w, h, hasil_img, 4);
    disp->imagePaste(img_ref, 0, 0, false);
  }
  
  disp->imageDelete(img_ref);
  free(hasil_img);
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
