#pkg-config --libs opencv

all: imageProcessing 

imageProcessing: imageProcessing.cpp globalVars.cpp imageProcessing.h routines.h utilities.o blockMatching.o loadImages.o cornerDetection.o transformImage.o filterImage.o xml.o histogram.o sensors.o
	g++ -g  imageProcessing.cpp -o imageProcessing -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -I/usr/include/libxml2 -lxml2 utilities.o blockMatching.o loadImages.o cornerDetection.o transformImage.o filterImage.o xml.o histogram.o sensors.o


utilities.o: utilities.cpp imageProcessing.h
	g++ -g -c utilities.cpp  -I/usr/include/libxml2

blockMatching.o: blockMatching.cpp imageProcessing.h
	g++ -g -O3 -c blockMatching.cpp  -I/usr/include/libxml2

cornerDetection.o: cornerDetection.cpp imageProcessing.h
	g++ -g -c cornerDetection.cpp  -I/usr/include/libxml2

transformImage.o: transformImage.cpp imageProcessing.h
	g++ -g -c transformImage.cpp  -I/usr/include/libxml2

filterImage.o: filterImage.cpp imageProcessing.h
	g++ -g -O3 -c filterImage.cpp  -I/usr/include/libxml2

loadImages.o: loadImages.cpp globalVars.h imageProcessing.h
	g++ -g -c loadImages.cpp  -I/usr/include/libxml2

xml.o: xml.cpp globalVars.h imageProcessing.h 
	g++ -g -c xml.cpp  -I/usr/include/libxml2

histogram.o: histogram.cpp globalVars.h imageProcessing.h 
	g++ -g -c histogram.cpp  -I/usr/include/libxml2

sensors.o: sensors.cpp globalVars.h imageProcessing.h 
	g++ -g -c sensors.cpp  -I/usr/include/libxml2


run0: imageProcessing
	./imageProcessing -v images samples

run1: imageProcessing
	./imageProcessing -v images/screen_12.yuv.png samples

run2: imageProcessing
	./imageProcessing -sh --irotate=20 --iscale=0.3 --sscale=0.8 --srotate=90 -v images/screen_12.yuv.png samples

run3: imageProcessing
	./imageProcessing -sh --blockmatching=1 -v images/screen_12.yuv.png samples

run4: imageProcessing
	./imageProcessing -sh --keypoints -v  images/screen_12.yuv.png samples

run5: imageProcessing
	./imageProcessing -sh --keypoints -v --ocv images/screen_12.yuv.png samples

run6: imageProcessing
	./imageProcessing -sh --keypoints --kmeans=4 -v --ocv images/screen_12.yuv.png samples

run7: imageProcessing
	./imageProcessing -sh  --xml=/Users/bodin/sampleCodes/sampleCodes/CudaCourse/ProjectCode/imageProcessingV2.dir/dec.xml --keypoints --kmeans=10 -v  images/screen_12.yuv.png samples

run8: imageProcessing
	 ./imageProcessing -v -sh --filter --blockmatching=1 images/screen_10.yuv.png  samples

run9: imageProcessing
	 ./imageProcessing -v -sh  --keypoints --kmeans=10 --ocv images/screen_8.yuv.png  samples/sample3.png

run10: imageProcessing
	 ./imageProcessing -v -sh  --blockmatching images/screen_9.yuv.png  samples/sample3.png

run11: imageProcessing
	 ./imageProcessing -v -sh  --histogram images/screen_9.yuv.png  samples/sample3.png

run12: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --blockmatching images/screen_9.yuv.png  samples/sample3.png

run13: imageProcessing
	 ./imageProcessing -v -sh  --sensors images/screen_9.yuv.png

run14: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --sensors images/flecheref.png 

run15: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --blockmatching OrdresDrone/screen_9.yuv.png  OrdresDrone/montes.png

run16: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --blockmatching OrdresDrone/screen_16.yuv.png  samples/montes.png

run17: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --blockmatching OrdresDrone/screen_17.yuv.png  samples/descends.png

run18: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --blockmatching OrdresDrone/screen_18.yuv.png  samples/droite.png

run19: imageProcessing
	 ./imageProcessing -v -sh  --histogram  --blockmatching OrdresDrone/screen_19.yuv.png  samples/gauche.png

clean:
	rm -r *~ imageProcessing *.dSYM *.o *.xml
