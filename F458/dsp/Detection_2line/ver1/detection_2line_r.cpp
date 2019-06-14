#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>

class LaneDetector {
private:
    double img_size;
    double img_center;
    double img_right_center;
    bool left_flag = false;
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
    cv::Mat mask(cv::Mat img_edges);
    std::vector<cv::Vec4i> houghLines(cv::Mat img_mask);
    std::vector<std::vector<cv::Vec4i> > lineSeparation(
        std::vector<cv::Vec4i> lines, cv::Mat img_edges);
    std::vector<cv::Point> regression(
        std::vector<std::vector<cv::Vec4i> > left_right_lines,
        cv::Mat inputImage);
    std::string predictTurn();
    int plotLane(cv::Mat inputImage, cv::Mat refer_img,
        std::vector<cv::Point> lane,
        std::string turn);
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
//        cv::threshold(output, output, 140, 255, cv::THRESH_BINARY);
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

cv::Mat LaneDetector::mask(cv::Mat img_edges) {
        cv::Mat output;
        cv::Mat mask = cv::Mat::zeros(img_edges.size(),
                    img_edges.type());

#if 0
        cv::Point pts[4] = {
                cv::Point(210, 720),
                cv::Point(550, 450),
                cv::Point(717, 450),
                cv::Point(1280, 720)
        };
#endif
        // 210 / 1280 ==>> x / 450, 73
        // 550 / 1280 ==>> x / 450, 193
        // 450 / 720  ==>> x / 300, 187
        // 717 / 1280 ==>> x / 450, 252
        cv::Point pts[4] = {
                cv::Point(200, 110),
                cv::Point(680, 110),
                cv::Point(680, 350),
                cv::Point(0, 350)
        };

        cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255, 0, 0));
        cv::bitwise_and(img_edges, mask, output);

        return output;
}

std::vector<cv::Vec4i> LaneDetector::houghLines(cv::Mat img_mask) {
        std::vector<cv::Vec4i> line;

        HoughLinesP(img_mask, line, 1, CV_PI/180, 20, 40, 30);

        return line;
}

std::vector<std::vector<cv::Vec4i> >
LaneDetector::lineSeparation(
    std::vector<cv::Vec4i> lines, cv::Mat img_edges) {
        std::vector<std::vector<cv::Vec4i> > output(3);
        size_t j = 0;
        cv::Point ini;
        cv::Point fini;
        double slope_thresh = 0.3;
        double second_bias= 4; //second line bias
        std::vector<double> slopes;
        std::vector<cv::Vec4i> selected_lines;
        std::vector<cv::Vec4i> right_lines, left_lines, right2_lines;

        for (auto i : lines) {
                ini = cv::Point(i[0], i[1]);
                fini = cv::Point(i[2], i[3]);

                double slope =
            (static_cast<double>(fini.y) -
            static_cast<double>(ini.y)) /
            (static_cast<double>(fini.x) -
            static_cast<double>(ini.x) +
            0.00001);

                if (std::abs(slope) > slope_thresh) {
                        slopes.push_back(slope);
                        selected_lines.push_back(i);
                }
        }

        img_center = static_cast<double>((img_edges.cols / 2));
        img_right_center =  static_cast<double>(3*(img_edges.cols / 4));
        while (j < selected_lines.size()) {
                ini = cv::Point(selected_lines[j][0],
                selected_lines[j][1]);
                fini = cv::Point(selected_lines[j][2],
                selected_lines[j][3]);

//               vertical(about degree 85) detection from slopes value
                if(std::abs(slopes[j]) > 11.43){

                }else if (slopes[j] > 0 &&
                        fini.x > img_center &&
                        ini.x > img_center) {
                            if(fini.x > (img_right_center + second_bias) &&
                                    ini.x > (img_right_center+second_bias)){
                                        right2_lines.push_back(selected_lines[j]);
                                        right2_flag = true;
                            }else if(fini.x < (img_right_center-second_bias)  &&
                                    ini.x <(img_right_center-second_bias)){
                                right_lines.push_back(selected_lines[j]);
                                right_flag = true;
                            }

                } else if (slopes[j] < 0 &&
                        fini.x < img_center &&
                        ini.x < img_center) {
                            left_lines.push_back(selected_lines[j]);
                            left_flag = true;
                }

                j++;
        }

        output[0] = right_lines;
        output[1] = left_lines;
        if(right2_flag = true)
            output[2] = right2_lines;

        return output;
}

std::vector<cv::Point> LaneDetector::regression(
    std::vector<std::vector<cv::Vec4i> > left_right_lines,
    cv::Mat inputImage) {
        std::vector<cv::Point> output(6);
        cv::Point ini;
        cv::Point fini;
        cv::Point ini2;
        cv::Point fini2;
        cv::Vec4d right_line;
        cv::Vec4d right2_line;
        cv::Vec4d left_line;
        std::vector<cv::Point> right_pts;
        std::vector<cv::Point> right_pts2;
        std::vector<cv::Point> left_pts;
        double right2_ini_x = 0;
        double right2_fin_x = 0;

        if (right_flag == true) {
                for (auto i : left_right_lines[0]) {
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

        if (left_flag == true) {
                for (auto j : left_right_lines[1]) {
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

        if (right2_flag == true) {
                for (auto k : left_right_lines[2]) {
                        ini = cv::Point(k[0], k[1]);
                        fini = cv::Point(k[2], k[3]);

                        right_pts2.push_back(ini);
                        right_pts2.push_back(fini);
                }

                if (right_pts2.size() > 0) {
                        cv::fitLine(right_pts2, right2_line,
                CV_DIST_L2, 0, 0.01, 0.01);
                        right2_m = right2_line[1] / right2_line[0];
                        right2_b = cv::Point(right2_line[2], right2_line[3]);
                }
        }


        int ini_y = inputImage.rows;
        int fin_y = 155;

        double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
        double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

        if(right2_flag == true){
        right2_ini_x = ((ini_y - right2_b.y) / right2_m) + right2_b.x;
        right2_fin_x = ((fin_y - right2_b.y) / right2_m) + right2_b.x;
        }

        double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
        double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;

        output[0] = cv::Point(right_ini_x, ini_y);
        output[1] = cv::Point(right_fin_x, fin_y);
        output[2] = cv::Point(left_ini_x, ini_y);
        output[3] = cv::Point(left_fin_x, fin_y);

        if(right2_flag == true){
        output[4] = cv::Point(right2_ini_x, ini_y);
        output[5] = cv::Point(right2_fin_x, fin_y);
        }

        return output;
}

std::string LaneDetector::predictTurn() {
        std::string output;
        double vanish_x;
        double thr_vp = 50; //origianl value = 10;

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

int LaneDetector::plotLane(cv::Mat inputImage, cv::Mat refer_img,
    std::vector<cv::Point> lane, std::string turn) {
        std::vector<cv::Point> poly_points;
        cv::Mat output;

        inputImage.copyTo(output);
        poly_points.push_back(lane[2]);
        poly_points.push_back(lane[4]);
        poly_points.push_back(lane[5]);
        poly_points.push_back(lane[3]);
        cv::fillConvexPoly(output, poly_points,
            cv::Scalar(0, 0, 255), CV_AA, 0);
        cv::addWeighted(output, 0.3, inputImage,
            1.0 - 0.3, 0, inputImage);

        cv::line(inputImage, lane[0], lane[1],
        cv::Scalar(0, 255, 255), 5, CV_AA);
        cv::line(inputImage, lane[2], lane[3],
        cv::Scalar(0, 255, 255), 5, CV_AA);

        if(right2_flag == true){
        cv::line(inputImage, lane[4], lane[5],
        cv::Scalar(0, 0, 255), 5, CV_AA);
        }

        cv::putText(inputImage, turn, cv::Point(50, 90),
        cv::FONT_HERSHEY_COMPLEX_SMALL, 3,
        cv::Scalar(0, 255, 0), 1, CV_AA);


//        cv::imwrite("2Lane_plot.jpg", inputImage);

        cv::namedWindow("Lane", CV_WINDOW_AUTOSIZE);
        cv::imshow("Lane", inputImage);
        cv::namedWindow("Mask_img", CV_WINDOW_AUTOSIZE);
        cv::imshow("Mask_img", refer_img);


        return 0;
}

int main(int argc, char *argv[]) {


    if (argc != 2) {
                std::cout << "Not enough parameters" << std::endl;
                return -1;
        }

        std::string source = argv[1];
        cv::VideoCapture cap(source);
        if (!cap.isOpened())
                return -1;




        LaneDetector lanedetector;
        cv::Mat frame;
        cv::Mat img_denoise;
        cv::Mat img_edges;
        cv::Mat img_mask;
        cv::Mat img_lines;
        std::vector<cv::Vec4i> lines;
        std::vector<std::vector<cv::Vec4i> > left_right_lines;
        std::vector<cv::Point> lane;
        std::vector<std::vector<cv::Vec4i> > slpoes_mean;
        std::string turn;
        int flag_plot = -1;
        int i = 0;
        int j = 0;


        while (i < 1200) {
                if (!cap.read(frame))
                        break;
                img_denoise = lanedetector.deNoise(frame);

                img_edges = lanedetector.edgeDetector(img_denoise);

                img_mask = lanedetector.mask(img_edges);

                lines = lanedetector.houghLines(img_mask);

                if (!lines.empty()) {
                        left_right_lines =
                               lanedetector.lineSeparation(lines, img_edges);

                        lane =
                               lanedetector.regression(left_right_lines, frame);

                        turn = lanedetector.predictTurn();

                        flag_plot = lanedetector.plotLane(frame, img_mask, lane, turn);


                        i += 1;
                        cv::waitKey(25);


                } else {
                        flag_plot = -1;
                }
        }
        return flag_plot;
}
