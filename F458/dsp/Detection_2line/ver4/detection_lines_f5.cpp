#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



class LaneDetector {
private:
    double img_size;
    double img_center;
    double img_center_bias;
    double img_right_center;
    bool left_flag = false;
    bool left2_flag = false;
    bool right_flag = false;
    bool right2_flag = false;
    cv::Point right_b;
    double right_m;          // y = m*x + b
    cv::Point right2_b;
    double right2_m;
    cv::Point left_b;
    double left_m;


public:
    cv::Mat deNoise(cv::Mat inputImage);
    cv::Mat edgeDetector(cv::Mat img_noise);
    cv::Mat mask(cv::Mat img_edges, int scene);
    std::vector<cv::Vec4i> houghLines(cv::Mat img_mask);
    std::vector<std::vector<cv::Vec4i> > lineSeparation(
        std::vector<cv::Vec4i> lines, cv::Mat img_edges);
    std::vector<cv::Point> regression(
        std::vector<std::vector<cv::Vec4i> > left_right_lines,
        cv::Mat inputImage);
    std::vector<cv::Point> regression2(
        std::vector<cv::Vec4i> lines,
        cv::Mat inputImage);
    std::string predictTurn();
    cv::Mat plotLane(cv::Mat inputImage,
        std::vector<cv::Point> lane,
        std::string turn, int line2_flag);
};

cv::Mat LaneDetector::deNoise(cv::Mat inputImage) {
        cv::Mat output;

        cv::GaussianBlur(inputImage, output, cv::Size(9, 9), 0, 0);

        return output;
}

cv::Mat LaneDetector::edgeDetector(cv::Mat img_noise) {
        cv::Mat output;
        cv::Mat kernel;
        cv::Point anchor;

        cv::cvtColor(img_noise, output, cv::COLOR_RGB2GRAY);
        //cv::threshold(output, output, 140, 255, cv::THRESH_BINARY);
        cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 11);

        anchor = cv::Point(-1, -1);
        kernel = cv::Mat(1, 3, CV_32F);
        kernel.at<float>(0, 0) = -1;
        kernel.at<float>(0, 1) = 0;
        kernel.at<float>(0, 2) = 1;

        cv::filter2D(output, output, -1, kernel,
            anchor, 0, cv::BORDER_DEFAULT);

        return output;
}

cv::Mat LaneDetector::mask(cv::Mat img_edges, int scene) {
        cv::Mat output;
        cv::Mat mask = cv::Mat::zeros(img_edges.size(),
                    img_edges.type());
        int ch = 0;

        if(scene == 0){
            ch = 4;
            cv::Point pts[4] = {
                             cv::Point(220, 200),
                             cv::Point(370, 200),
                             cv::Point(600, 460),
                             cv::Point(60, 460)
            };
                   cv::fillConvexPoly(mask, pts, ch, cv::Scalar(255, 0, 0));

        }
        if(scene == 1){
                   ch = 3;
                   cv::Point pts[3] = {
                               cv::Point(420, 250),
                               cv::Point(610, 250),
                               cv::Point(610, 460)
                   };
                   cv::fillConvexPoly(mask, pts, ch, cv::Scalar(255, 0, 0));
       }
       if(scene == 2){
                   ch = 3;
                   cv::Point pts[3] = {
                               cv::Point(180, 250),
                               cv::Point(10, 250),
                               cv::Point(10, 460)
                   };
                    cv::fillConvexPoly(mask, pts, ch, cv::Scalar(255, 0, 0));
       }
       if(scene == 3){
                    ch = 4;
                    cv::Point pts[4] = {
                                cv::Point(10, 250),
                                cv::Point(420, 250),
                                cv::Point(610, 420),
                                cv::Point(10, 420)
                    };
                   cv::fillConvexPoly(mask, pts, ch, cv::Scalar(255, 0, 0));
        }
        if(scene == 4){
                     ch = 4;
                     cv::Point pts[4] = {
                                 cv::Point(200, 250),
                                 cv::Point(610, 250),
                                 cv::Point(610, 420),
                                 cv::Point(10, 420)
                     };
                     cv::fillConvexPoly(mask, pts, ch, cv::Scalar(255, 0, 0));
        }

        cv::bitwise_and(img_edges, mask, output);

        return output;
}

std::vector<cv::Vec4i> LaneDetector::houghLines(cv::Mat img_mask) {
        std::vector<cv::Vec4i> line;

        HoughLinesP(img_mask, line, 1, CV_PI/180, 20, 25, 8);

        return line;
}

std::vector<std::vector<cv::Vec4i> >
LaneDetector::lineSeparation(
    std::vector<cv::Vec4i> lines, cv::Mat img_edges) {
    std::vector<std::vector<cv::Vec4i> > output(2);
        size_t j = 0;
        cv::Point ini;
        cv::Point fini;
        double slope_thresh_mini = 0.36;
        double slope_thresh_max = 28.63;
        std::vector<double> slopes;
        std::vector<cv::Vec4i> selected_lines;
        std::vector<cv::Vec4i> right_lines, left_lines;

        for (auto i : lines) {
                ini = cv::Point(i[0], i[1]);
                fini = cv::Point(i[2], i[3]);

                double slope =
            (static_cast<double>(fini.y) -
            static_cast<double>(ini.y)) /
            (static_cast<double>(fini.x) -
            static_cast<double>(ini.x) +
            0.00001);

                // vertical(about degree 88) & horizon(20) detection from slopes value
                if ((std::abs(slope) > slope_thresh_mini) && (std::abs(slope) < slope_thresh_max))
                {
                        slopes.push_back(slope);
                        selected_lines.push_back(i);
                }
        }

        img_center_bias = -55; //when test video, use this code
                               //when live video, img_center_bias value must be 0
        img_center = static_cast<double>((img_edges.cols / 2))+img_center_bias;

//      img_center = 275;
//        std::cout<<"img_center "<<img_center<<std::endl;
//      img_right_center =  static_cast<double>(3*(img_edges.cols / 4))-second_bias;
        while (j < selected_lines.size()) {
                ini = cv::Point(selected_lines[j][0],
                selected_lines[j][1]);
                fini = cv::Point(selected_lines[j][2],
                selected_lines[j][3]);

                if (slopes[j] < 0 &&
                         fini.x < img_center &&
                         ini.x < img_center) {
                                  left_lines.push_back(selected_lines[j]);
                                  left_flag = true;
                }
                else if (slopes[j] > 0 &&
                        fini.x > img_center &&
                        ini.x > img_center) {
                            right_lines.push_back(selected_lines[j]);
                            right_flag = true;
                }
                j++;
        }

        output[0] = left_lines;
        output[1] = right_lines;

        return output;
}

std::vector<cv::Point> LaneDetector::regression(
    std::vector<std::vector<cv::Vec4i> > left_right_lines,
    cv::Mat inputImage) {
        std::vector<cv::Point> output(4);
        cv::Point ini;
        cv::Point fini;
        cv::Point ini2;
        cv::Point fini2;
        cv::Vec4d right_line;
        cv::Vec4d left_line;
        std::vector<cv::Point> right_pts;
        std::vector<cv::Point> left_pts;

        if (left_flag == true) {
                for (auto j : left_right_lines[0]) {
                        ini2 = cv::Point(j[0], j[1]);
                        fini2 = cv::Point(j[2], j[3]);

                        left_pts.push_back(ini2);
                        left_pts.push_back(fini2);
                }

                if (left_pts.size() > 0) {
                        cv::fitLine(left_pts, left_line,
                CV_DIST_L2, 0, 0.01, 0.01);
                        left_m = left_line[1] / left_line[0];
                        left_b = cv::Point(left_line[2], left_line[3]);
                }
        }

        if (right_flag == true) {
                for (auto i : left_right_lines[1]) {
                        ini = cv::Point(i[0], i[1]);
                        fini = cv::Point(i[2], i[3]);

                        right_pts.push_back(ini);
                        right_pts.push_back(fini);
                }

                if (right_pts.size() > 0) {
                        cv::fitLine(right_pts, right_line,
                CV_DIST_L2, 0, 0.01, 0.01);
                        right_m = right_line[1] / right_line[0];
                        right_b = cv::Point(right_line[2], right_line[3]);
                }
        }

        int ini_y = 380;
        int fin_y = 100;

        double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
        double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

        double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
        double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;

        output[0] = cv::Point(right_ini_x, ini_y);
        output[1] = cv::Point(right_fin_x, fin_y);

        output[2] = cv::Point(left_ini_x, ini_y);
        output[3] = cv::Point(left_fin_x, fin_y);

        return output;
}

std::vector<cv::Point> LaneDetector::regression2(
         std::vector<cv::Vec4i> lines,
         cv::Mat inputImage){
            std::vector<cv::Point> output(2);
            std::vector<cv::Point> line_pts2;
            cv::Point ini;
            cv::Point fini;
            cv::Vec4d line2;
            double right2_ini_x = 0;
            double right2_fin_x = 0;

            for (auto k : lines) {
                ini = cv::Point(k[0], k[1]);
                fini = cv::Point(k[2], k[3]);

                line_pts2.push_back(ini);
                line_pts2.push_back(fini);

                if (line_pts2.size() > 0) {
                    cv::fitLine(line_pts2, line2,
                                CV_DIST_L2, 0, 0.01, 0.01);
                    right2_m = line2[1] / line2[0];
                    right2_b = cv::Point(line2[2], line2[3]);
                    right2_flag = true;
               }
           }

           int ini_y = 380;
           int fin_y = 100;

           right2_ini_x = ((ini_y - right2_b.y) / right2_m) + right2_b.x;
           right2_fin_x = ((fin_y - right2_b.y) / right2_m) + right2_b.x;

           output[0] = cv::Point(right2_ini_x, ini_y);
           output[1] = cv::Point(right2_fin_x, fin_y);

           return output;
}

std::string LaneDetector::predictTurn() {
        std::string output;
        double vanish_x;
        double vanish_y;
        double thr_vp = 40; //original value = 10;

        vanish_x = static_cast<double>(((right_m*right_b.x) -
    (left_m*left_b.x) - right_b.y + left_b.y) / (right_m - left_m));

            if (vanish_x < (img_center - thr_vp))
                    output = "Left Turn";
            else if (vanish_x > (img_center + thr_vp))
                    output = "Right Turn";
            else if (vanish_x >= (img_center - thr_vp) &&
                    vanish_x <= (img_center + thr_vp))
                    output = "Straight";

        return output;
}

cv::Mat LaneDetector::plotLane(cv::Mat inputImage,
    std::vector<cv::Point> lane, std::string turn, int line2_flag) {
        cv::Mat output;

        inputImage.copyTo(output);

        if(right_flag == true){
        cv::line(output, lane[0], lane[1],
        cv::Scalar(255, 0, 0), 5, CV_AA);
        }
        if(left_flag ==  true){
        cv::line(output, lane[2], lane[3],
        cv::Scalar(0, 255, 0), 5, CV_AA);
        }

        if(line2_flag == 1){
            cv::putText(output, "Detect Right Lane", cv::Point(50, 240),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 2,
                        cv::Scalar(100, 50, 50), 1, CV_AA);
        }
        if(line2_flag == 2){
            cv::putText(output, "Detect Left Lane", cv::Point(50, 240),
                         cv::FONT_HERSHEY_COMPLEX_SMALL, 2,
                         cv::Scalar(100, 50, 50), 1, CV_AA);
        }

        cv::putText(output, turn, cv::Point(50, 90),
        cv::FONT_HERSHEY_COMPLEX_SMALL, 3,
        cv::Scalar(0, 200, 0), 1, CV_AA);

        left_flag = false;
        right_flag = false;

        return output;
}




cv::Mat img_result(LaneDetector lanedetector, cv::Mat frame, int i){

    cv::Mat img_out;
    cv::Mat img_denoise;
    cv::Mat img_edges;
    cv::Mat img_mask;
    cv::Mat img_mask1;
    cv::Mat img_mask2;
    cv::Mat img_lines;
    std::vector<cv::Vec4i> lines;
    std::vector<cv::Vec4i> lines1;
    std::vector<cv::Vec4i> lines2;
    std::vector<std::vector<cv::Vec4i> > left_right_lines;
    std::vector<cv::Point> lane;
    std::vector<cv::Point> lane1;
    std::vector<cv::Point> lane2;
    std::string turn;
    int scene = 0;
    int move_side = 0;
    int line2_flag = 0; // 1 is right
                        // 2 is left

                   frame.copyTo(img_out);
                   std::cout<<"i =  "<<i<<std::endl;

                   img_denoise = lanedetector.deNoise(frame);
                   img_edges = lanedetector.edgeDetector(img_denoise);

                   if((i > 120) && (i <230)){ // first changing
                       move_side = 1;
                       scene = 3;
                   }
                   if((i>230) && (i<300)){ // changed line
                       move_side = 0;
                   }
                   if((i>300) && (i<420)){ //second changing
                       move_side = 1;
                       scene = 4;
                   }
                   if(i>420){ // changed line
                       move_side = 0;
                   }


                   //junction point
                   // move_side == 0 start point
                   if(move_side == 0){
                   scene = 0;
                   line2_flag = 0;

                   //center detection
                   img_mask = lanedetector.mask(img_edges, scene);
                   lines = lanedetector.houghLines(img_mask);

                   if(scene==0){
                       if (!lines.empty()) {
                           left_right_lines =
                                  lanedetector.lineSeparation(lines, frame);
                           lane =
                                  lanedetector.regression(left_right_lines, frame);

                           turn = lanedetector.predictTurn();
                       }
                   }

                   //right detection
                   scene = 1;
                   img_mask1 = lanedetector.mask(img_edges, scene);
                   lines1 = lanedetector.houghLines(img_mask1);
                   //left detection
                   scene = 2;
                   img_mask2 = lanedetector.mask(img_edges, scene);
                   lines2 = lanedetector.houghLines(img_mask2);

                   if(lines1.size() >= lines2.size()){
                       if(!lines1.empty()){
                           lane1 = lanedetector.regression2(lines1, frame);

                           line(frame, lane1[0], lane1[1],
                                 cv::Scalar(0,0,255), 2, cv::LINE_AA );

                           line2_flag =1;
                           std::cout<<"line2_flag "<<line2_flag<<std::endl;
                       }else{
                           std::cout<<"Don't detect line2"<<std::endl;
                       }

                   }else if(!(lines1.size() >= lines2.size())){
                       if (!lines2.empty()) {
                           lane2 = lanedetector.regression2(lines2, frame);

                           line(frame, lane2[0], lane2[1],
                                cv::Scalar(0,0,255), 2, cv::LINE_AA );

                           line2_flag =2;
                           std::cout<<"line2_flag "<<line2_flag<<std::endl;
                       }else{
                           std::cout<<"Don't detect line2"<<std::endl;
                       }

                   }
                   // assemble roi(0~2)
                   img_out = lanedetector.plotLane(frame, lane, turn, line2_flag);


                   }else{// move_side == 0 end point
                   //move_side == 1 start point
                       std::cout<<"scene "<<scene<<std::endl;
                       img_mask = lanedetector.mask(img_edges, scene);
                       lines = lanedetector.houghLines(img_mask);

                       if (!lines.empty()){
                           for( int j=0; j<lines.size(); j++)
                           {
                               cv::Vec4i L = lines[j];
                               line(img_out, cv::Point(L[0],L[1]), cv::Point(L[2],L[3]),
                                    cv::Scalar(255,255,0), 2, cv::LINE_AA );

                               cv::putText(img_out, "ChangingLanes", cv::Point(50, 120),
                                           cv::FONT_HERSHEY_COMPLEX_SMALL, 2,
                                           cv::Scalar(100, 50, 50), 1, CV_AA);
                           }
                       }else{
                           std::cout<<"Don't detect lines"<<std::endl;
                       }

                   }// move_side == 1 end point

            //while end
        return img_out;
}

void * img_func(void *data){

           std::string video_name =  *static_cast<std::string*>(data);
           cv::VideoCapture cap(video_name);
           if (!cap.isOpened())
               std::cout << "cap don't open" << std::endl;

           LaneDetector lanedetector;
           cv::Mat input_frame;
           cv::Mat output_frame;
           int i=0;

           while (cap.read(input_frame)) {

           output_frame = img_result(lanedetector, input_frame, i);

           cv::imshow("myline_plot", output_frame);
           cv::waitKey(20);

           i++;
           }
}

int main(int argc, char *argv[]) {
        if (argc != 2) {
                std::cout << "Not enough parameters" << std::endl;
                return -1;
        }
        std::string source = argv[1];

        pthread_t p_thread[2];
        int thr_id;
        int status;

        thr_id = pthread_create(&p_thread[0], NULL, img_func, &source);
        if(thr_id < 0){
            perror("img func thread create error");
            exit(0);
        }

        pthread_join(p_thread[0], (void **)status);

        return 0;
}

