#include "CameraImageWrapper.h"
#include <QColor>
#include <QApplication>
#include <QDesktopWidget>

CameraImageWrapper::CameraImageWrapper() : LuminanceSource(0,0), image(NULL)
{
}

CameraImageWrapper::CameraImageWrapper(const QImage &sourceImage) : LuminanceSource(sourceImage.width(), sourceImage.height())
{
    image = grayScaleImage( &sourceImage );
    delegate = Ref<GreyscaleLuminanceSource>(
                new GreyscaleLuminanceSource(getMatrixP(),image->width(), image->height(),0, 0, image->width(), image->height()));
}

CameraImageWrapper::CameraImageWrapper(CameraImageWrapper& otherInstance) : LuminanceSource(otherInstance.getWidth(), otherInstance.getHeight())
{
    image = new QImage(otherInstance.getOriginalImage());
    delegate = otherInstance.getDelegate();
}

CameraImageWrapper::~CameraImageWrapper()
{
    if(image)
        delete image;
}

CameraImageWrapper *CameraImageWrapper::Factory(const QImage &sourceImage, int maxWidth, int maxHeight, bool smoothTransformation)
{
    if((maxWidth != 1 && sourceImage.width() > maxWidth) || (maxHeight != 1 && sourceImage.height() > maxHeight))
    {
        QImage image;
        image = sourceImage.scaled(
                    maxWidth != -1 ? maxWidth : sourceImage.width(),
                    maxHeight != -1 ? maxHeight : sourceImage.height(),
                    Qt::KeepAspectRatio,
                    smoothTransformation ? Qt::SmoothTransformation : Qt::FastTransformation);
        return new CameraImageWrapper(image);
    }
    else
        return new CameraImageWrapper(sourceImage);
}

QImage* CameraImageWrapper::grayScaleImage(const QImage *origin)
{
    QImage *tmp = new QImage(origin->width(), origin->height(), QImage::Format_Grayscale8);
    for(int i=0; i<origin->width(); i++)
    {
        for(int j=0; j<origin->height(); j++)
        {
            QRgb pixel = origin->pixel(QPoint(i,j));
            QRgb pix = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3;
                    //qGray(origin->pixel(QPoint(i,j)));
            tmp->setPixel(i,j, qRgb(pix ,pix,pix));
        }
    }

    return tmp;
}

QImage CameraImageWrapper::getOriginalImage()
{
    return *image;
}

ArrayRef<char> CameraImageWrapper::getRow(int y, ArrayRef<char> row) const
{
    if(delegate)
        return delegate->getRow(y, row);
    else
        return getRowP(y, row);
}

ArrayRef<char> CameraImageWrapper::getMatrix() const
{
    if(delegate)
        return delegate->getMatrix();
    else
        return getMatrixP();
}

bool CameraImageWrapper::isCropSupported() const
{
    if(delegate)
        return delegate->isCropSupported();
    else
        return LuminanceSource::isCropSupported();
}

Ref<LuminanceSource> CameraImageWrapper::crop(int left, int top, int width, int height) const
{
    if(delegate)
        return delegate->crop(left, top, width, height);
    else
        return LuminanceSource::crop(left, top, width, height);
}

bool CameraImageWrapper::isRotateSupported() const
{
    if(delegate)
        return delegate->isRotateSupported();
    else
        return LuminanceSource::isRotateSupported();
}

Ref<LuminanceSource> CameraImageWrapper::invert() const
{
    if(delegate)
        return delegate->invert();
    else
        return LuminanceSource::invert();
}

Ref<LuminanceSource> CameraImageWrapper::rotateCounterClockwise() const
{
    if(delegate)
        return delegate->rotateCounterClockwise();
    else
        return LuminanceSource::rotateCounterClockwise();
}

ArrayRef<char> CameraImageWrapper::getRowP(int y, ArrayRef<char> row) const
{
    int width = getWidth();

    if (row->size() != width)
        row.reset(ArrayRef<char>(width));

    for (int x = 0; x < width; x++)
        row[x] = image->pixel(x,y);

    return row;
}

ArrayRef<char> CameraImageWrapper::getMatrixP() const
{
    int width = getWidth();
    int height =  getHeight();

    ArrayRef<char> tmpRow(0);
    ArrayRef<char> arr(width*height);

    char* m = &arr[0];

    for(int y=0; y<height; y++)
    {
        tmpRow = getRowP(y, tmpRow);
#if __cplusplus > 199711L
        memcpy(m, tmpRow->values().data(), width);
#else
        memcpy(m, &tmpRow[0], width);
#endif
        m += width * sizeof(unsigned char);
    }

    return arr;
}

//bool CameraImageWrapper::isCropSupported() const
//{
//    return true;
//}

//Ref<LuminanceSource> CameraImageWrapper::crop(int left, int top, int width, int height) const
//{

//}

//bool CameraImageWrapper::isRotateSupported() const
//{
//    return true;
//}

//Ref<LuminanceSource> CameraImageWrapper::invert() const
//{

//}

//Ref<LuminanceSource> CameraImageWrapper::rotateCounterClockwise() const
//{

//}

QImage *CameraImageWrapper::sharpen(const QImage *origin)
{
    QImage * newImage = new QImage(* origin);

    int kernel [3][3]= {{0,-1,0},
                        {-1,5,-1},
                        {0,-1,0}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r,g,b;
    QColor color;

    for(int x=kernelSize/2; x<newImage->width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){

            r = 0;
            g = 0;
            b = 0;

            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    color = QColor(origin->pixel(x+i, y+j));
                    r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }

            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));

        }
    }
    return newImage;
}