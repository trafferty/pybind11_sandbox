/*
 * ip_routines.h
 *
 *  Created on: Sep 11, 2015
 *      Author: trafferty
 */

#include <vector>
#include <utility>      // std::pair
#include <tuple>

// OpenCV:
#include "opencv2/core.hpp"

// from common:
#include "Logger.h"

#ifndef INCLUDE_IP_ROUTINES_H_
#define INCLUDE_IP_ROUTINES_H_

using namespace std;
using namespace cv;

// typdefs:
typedef std::pair<cv::Point2f, cv::Point2f> pt_set_t;
typedef std::vector< pt_set_t > pt_sets_t;

// contants:
const Scalar white = Scalar(255, 255, 255);
const Scalar black = Scalar(0, 0, 0);
const Scalar blue = Scalar(255, 0, 0);
const Scalar navy = Scalar(128, 0, 0);
const Scalar gray = Scalar(144, 128, 112);
const Scalar green = Scalar(0, 255, 0);
const Scalar red = Scalar(0, 0, 255);
const Scalar yellow = Scalar(0, 255, 255);
const Scalar magenta = Scalar(238, 0, 238);
const Scalar orange = Scalar(0, 154, 238);

class circle_coord {
public:
   circle_coord(double _x, double _y, double _r) {
      centerPt.x = _x;
      centerPt.y = _y;
      r = _r;
   }
   circle_coord(cv::Point _centerPt, double _r) {
      centerPt.x = _centerPt.x;
      centerPt.y = _centerPt.y;
      r = _r;
   }
   cv::Point centerPt;
   double r;
};

void printMatInfo(const cv::Mat& mat, string name);

double stdev_vec(const vector<double>& v);

cv::Point2f polar2cart(double r, double theta_rad, cv::Point centerPt);

cv::Point2f findAvePt(const vector<cv::Point2f>& pts);

std::vector<cv::Point2f> findPtsOnCircle(cv::Point2f centerXY, double r);

void rotate(const cv::Mat& src, cv::Mat& dst, int angle);

void subimage(const cv::Mat& img, cv::Mat& patch, cv::Point2f centerPt, double theta, int width, int height);

int findFirstPeakAboveThresh(const vector<uint>& range,  uint thresh);

std::tuple< double, pt_sets_t > estimateCircleDiameter(const cv::Mat& img, cv::Point2f centerPt, double diag_len, std::unique_ptr<Logger> logger);

#endif /* INCLUDE_IP_ROUTINES_H_ */
