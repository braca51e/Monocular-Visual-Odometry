

#ifndef FRAME_H
#define FRAME_H

#include "my_slam/common_include.h"
#include "my_basics/opencv_funcs.h"
#include "my_geometry/camera.h"
#include "my_geometry/feature_match.h"
using namespace std;
using namespace cv;
using namespace my_basics;
using namespace my_geometry;

namespace my_slam
{

class Frame
{
private:
public:
  typedef std::shared_ptr<Frame> Ptr;
  static unsigned long factory_id_;

public:
  unsigned long id_;  // id of this frame
  double time_stamp_; // when it is recorded

  // image features
  Mat rgb_img_;
  vector<KeyPoint> keypoints_;
  Mat descriptors_;
  vector<vector<unsigned char>> kpts_colors_; // rgb colors

  vector<DMatch> matches_;        // matches with the previous frame
  vector<DMatch> inlier_matches_; // inliers matches index with respect to all the points
  vector<Point3f> inliers_pts3d_; // matches with the previous frame

  // vector<int> inliers_of_matches_; // inliers index with respect to the matches
  // vector<int> inliers_of_all_pts_; // inliers index with respect to all the points

  // -- Other commonly used points
  // vector<Point2f> pts_matched_;

  // pose
  my_geometry::Camera::Ptr camera_;
  Mat T_w_c_; // transform from camera to world
  // Mat R_curr_to_prev_;
  // Mat t_curr_to_prev_;

public:
  Frame() {}
  ~Frame() {}
  static Frame::Ptr createFrame(Mat rgb_img, my_geometry::Camera::Ptr camera, double time_stamp = -1);

public: // Below are deprecated. These were used in the two-frame-matching vo.
  void extractKeyPoints()
  {
    my_geometry::extractKeyPoints(rgb_img_, keypoints_);
  }
  void computeDescriptors()
  {
    my_geometry::computeDescriptors(rgb_img_, keypoints_, descriptors_);
    kpts_colors_.clear();
    for (KeyPoint kpt : keypoints_)
    {
      int x = floor(kpt.pt.x), y = floor(kpt.pt.y);
      kpts_colors_.push_back(getPixelAt(rgb_img_, x, y));
    }
  };
  void matchFeatures(Frame::Ptr prev_frame)
  {
    my_geometry::matchFeatures(
        // descriptors_, prev_frame->descriptors_,
        prev_frame->descriptors_, descriptors_,
        matches_,
        true // print result
    );
  }
  bool isInFrame(const Point3f &p_world);
  bool isInFrame(const Mat &p_world);
  Mat getCamCenter();
  
};

} // namespace my_slam

#endif // FRAME_H
