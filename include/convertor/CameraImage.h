#ifndef CAMERA_IMAGE_H
#define CAMERA_IMAGE_H

#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>
#include <cstdint>
#include <cstring>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

class ImageSizeException : std::exception {
  public:
    const char* what() const throw() {
        return "Invalid image size values";
    }
};

class ImagePositionException : std::exception {
  public:
    const char* what() const throw() {
        return "Invalid image position (y, x)";
    }
};

/*
 * Stores distances for all visible objects
 */

class CameraImage {
  public:
    CameraImage() {}
    CameraImage(int h, int w) : _h(h), _w(w) {
        if (_h <= 0 || _w <= 0)
            throw ImageSizeException();
        _dist = std::vector<uint16_t>(_h * _w, MAX_DIST);
    }
    virtual ~CameraImage() {}

    virtual void update(int i, int j, uint16_t d) {
        if (i < 0 || i >= _h || j < 0 || j >= _w)
            throw ImagePositionException();
        _dist[i * _w + j] = std::min(_dist[i * _w + j], d);
    }
    virtual uint16_t get(int i, int j) const {
        if (i < 0 || i >= _h || j < 0 || j >= _w)
            throw ImagePositionException();
        return _dist[i * _w + j];
    }

    virtual void write(std::ostream& out) const {
        for (int i = 0; i < _h; i++) {
            for (int j = 0; j < _w; j++)
                out << _dist[i * _w + j] << ' ';
            out << '\n';
        }
        out.flush();
    }
    virtual sensor_msgs::Image createImageMessage() const {
        sensor_msgs::Image img;
        img.header.seq = 0;
        img.header.stamp.sec = time(0);
        img.header.stamp.nsec = 0;
        img.header.frame_id = "image_rect";
        
        img.height = _h;
        img.width = _w;

        img.encoding = sensor_msgs::image_encodings::MONO16;

        img.step = 2 * _w;
        img.is_bigendian = 0;

        img.data = sensor_msgs::Image::_data_type(img.step * img.height);
        memcpy(&img.data[0], &_dist[0], img.step * img.height);
        
        return img;
    }

    virtual int getHeight() const {
        return _h;
    }
    virtual int getWidth() const {
        return _w;
    }

    static const int MAX_DIST;

  private:
    int _h, _w; // camera resolution
    std::vector<uint16_t> _dist; // array of distances
};

const int CameraImage::MAX_DIST = UINT16_MAX;

#endif
