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

using namespace cv;
using namespace webots;
using namespace std;

static unsigned char *hasil_img;
static int w;
static int h;

// static Scalar lMargin[NB_FILTERS] = {Scalar(0, 150, 30),  Scalar(58, 150, 30),  Scalar(115, 150, 30),
                                     // Scalar(28, 150, 30), Scalar(148, 150, 30), Scalar(0, 0, 50)};
// static Scalar uMargin[NB_FILTERS] = {Scalar(5, 255, 255),  Scalar(62, 255, 255),  Scalar(120, 255, 255),
                                     // Scalar(32, 255, 255), Scalar(152, 255, 255), Scalar(0, 0, 255)};

// static bool filters[NB_FILTERS + 1] = {false, false, false, false, false, false, true};
void process_image(const unsigned char *image, int length) {
  Mat img = Mat(Size(w, h), CV_8UC4);
  img.data = (uchar *)image;

  Mat hsv = Mat(Size(w, h), CV_8UC3);
  cvtColor(img, hsv, COLOR_BGR2HSV);

  Mat sementara_terfilt = Mat(Size(w, h), CV_8UC1);

  Mat terfilt = Mat(Size(w, h), CV_8UC4);

  // if (filters[NB_FILTERS]) {
    // terfilt = img;
  // } else {
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        terfilt.at<Vec4b>(i, j)[0] = 0;
        terfilt.at<Vec4b>(i, j)[1] = 0;
        terfilt.at<Vec4b>(i, j)[2] = 0;
        terfilt.at<Vec4b>(i, j)[3] = 255;
      }
    }

    // for (int f = 0; f < NB_FILTERS; ++f) {
      // if (filters[f]) {
        inRange(hsv, Scalar(94, 80, 2), Scalar(126, 255, 255), sementara_terfilt);
        for (int i = 0; i < h; ++i) {
          for (int j = 0; j < w; ++j) {
            if (sementara_terfilt.at<uchar>(i, j) == 255) {
              terfilt.at<Vec4b>(i, j) = img.at<Vec4b>(i, j);
            }
          }
        }
      // }
    // }
  //}

  memcpy(hasil_img, terfilt.data, length);
}

/* This function handles which filters need to be used when the display function is called. */
// void apply_filter(int filter) {
  // if (filter > (NB_FILTERS + 1)) {
    // printf("Error: Unknown filter.\n");
  // } else {
    // int cnt = 0;

    // if (filter == NB_FILTERS) {
      // for (int i = 0; i < NB_FILTERS; ++i)
        // filters[i] = false;
    // } else if (filter == (NB_FILTERS + 1)) {
      // for (int i = 0; i < NB_FILTERS; ++i)
        // filters[i] = true;
    // } else {
      // if (filters[filter])
        // filters[filter] = false;
      // else
        // filters[filter] = true;
    // }

    // printf("Filters currently applied: ");
    // for (int i = 0; i < NB_FILTERS; ++i) {
      // if (filters[i]) {
        // switch (i) {
          // case 0:
            // printf("red ");
            // break;
          // case 1:
            // printf("green ");
            // break;
          // case 2:
            // printf("blue ");
            // break;
          // case 3:
            // printf("yellow ");
            // break;
          // case 4:
            // printf("purple ");
            // break;
          // case 5:
            // printf("white ");
            // break;
          // default:
            // break;
        // }
        // ++cnt;
      // }
    // }

    // if (!cnt) {
      // printf("none (the entire image will be displayed).");
      // filters[NB_FILTERS] = true;
    // } else {
      // filters[NB_FILTERS] = false;
    // }

    // printf("\n");
  // }
// }

int main(int argc, char **argv) {
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  Motor *wheels[4], *capitm[2];
  Motor *sodoker;
  Camera *cam;
  Display *disp;
  ImageRef *img_ref;
  
  double p_a=0, p_b=0;
  int ukuran;
  char dsNames[3][10] = {"ds_right", "ds_left", "ds_top"};
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  char capit[2][8] = {"capit_a", "capit_b"};
  int avoidObstacleCounter = 0;
  const unsigned char *img;
  
  sodoker = robot->getMotor("sodok");
  cam = robot->getCamera("camera");
  
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
  img = cam->getImage();
  
  disp = robot->getDisplay("display");
  ukuran = 4*w*h*sizeof(unsigned char);
  hasil_img = (unsigned char *)malloc(ukuran);
  
  while (robot->step(TIME_STEP) != -1) {
    double leftSpeed = 1.0;
    double rightSpeed = 1.0;
    if (avoidObstacleCounter > 0) {
      avoidObstacleCounter--;
      leftSpeed = 1.0;
      rightSpeed = -1.0;
      capitm[0]->setPosition(p_b+1);
      capitm[1]->setPosition(p_b-1);
    } else { // read sensors
      for (int i = 0; i < 3; i++) {
        if (ds[i]->getValue() < 1000.0)
          avoidObstacleCounter = 60;
        if(i<=1)
          capitm[i]->setPosition(p_b);
      }
    }
    wheels[0]->setVelocity(leftSpeed);
    wheels[1]->setVelocity(rightSpeed);
    wheels[2]->setVelocity(leftSpeed);
    wheels[3]->setVelocity(rightSpeed);
    
    process_image(cam->getImage(), ukuran);
  
    img_ref = disp->imageNew(w, h, hasil_img, 3);
    disp->imagePaste(img_ref, 0, 0, false);
  }
  
  disp->imageDelete(img_ref);
  free(hasil_img);  
  delete robot;
  return 0;  // EXIT_SUCCESS
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
