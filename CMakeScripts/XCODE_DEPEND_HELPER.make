# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist
/usr/local/lib/libopencv_calib3d.dylib:
/usr/local/lib/libopencv_contrib.dylib:
/usr/local/lib/libopencv_core.dylib:
/usr/local/lib/libopencv_features2d.dylib:
/usr/local/lib/libopencv_flann.dylib:
/usr/local/lib/libopencv_gpu.dylib:
/usr/local/lib/libopencv_highgui.dylib:
/usr/local/lib/libopencv_imgproc.dylib:
/usr/local/lib/libopencv_legacy.dylib:
/usr/local/lib/libopencv_ml.dylib:
/usr/local/lib/libopencv_nonfree.dylib:
/usr/local/lib/libopencv_objdetect.dylib:
/usr/local/lib/libopencv_ocl.dylib:
/usr/local/lib/libopencv_photo.dylib:
/usr/local/lib/libopencv_stitching.dylib:
/usr/local/lib/libopencv_superres.dylib:
/usr/local/lib/libopencv_ts.dylib:
/usr/local/lib/libopencv_video.dylib:
/usr/local/lib/libopencv_videostab.dylib:


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.mapTest.Debug:
/Users/zhangzhao/Hecate/sources/mapTest/Debug/mapTest:\
	/usr/local/lib/libopencv_calib3d.dylib\
	/usr/local/lib/libopencv_contrib.dylib\
	/usr/local/lib/libopencv_core.dylib\
	/usr/local/lib/libopencv_features2d.dylib\
	/usr/local/lib/libopencv_flann.dylib\
	/usr/local/lib/libopencv_gpu.dylib\
	/usr/local/lib/libopencv_highgui.dylib\
	/usr/local/lib/libopencv_imgproc.dylib\
	/usr/local/lib/libopencv_legacy.dylib\
	/usr/local/lib/libopencv_ml.dylib\
	/usr/local/lib/libopencv_nonfree.dylib\
	/usr/local/lib/libopencv_objdetect.dylib\
	/usr/local/lib/libopencv_ocl.dylib\
	/usr/local/lib/libopencv_photo.dylib\
	/usr/local/lib/libopencv_stitching.dylib\
	/usr/local/lib/libopencv_superres.dylib\
	/usr/local/lib/libopencv_ts.dylib\
	/usr/local/lib/libopencv_video.dylib\
	/usr/local/lib/libopencv_videostab.dylib
	/bin/rm -f /Users/zhangzhao/Hecate/sources/mapTest/Debug/mapTest


PostBuild.mapTest.Release:
/Users/zhangzhao/Hecate/sources/mapTest/Release/mapTest:\
	/usr/local/lib/libopencv_calib3d.dylib\
	/usr/local/lib/libopencv_contrib.dylib\
	/usr/local/lib/libopencv_core.dylib\
	/usr/local/lib/libopencv_features2d.dylib\
	/usr/local/lib/libopencv_flann.dylib\
	/usr/local/lib/libopencv_gpu.dylib\
	/usr/local/lib/libopencv_highgui.dylib\
	/usr/local/lib/libopencv_imgproc.dylib\
	/usr/local/lib/libopencv_legacy.dylib\
	/usr/local/lib/libopencv_ml.dylib\
	/usr/local/lib/libopencv_nonfree.dylib\
	/usr/local/lib/libopencv_objdetect.dylib\
	/usr/local/lib/libopencv_ocl.dylib\
	/usr/local/lib/libopencv_photo.dylib\
	/usr/local/lib/libopencv_stitching.dylib\
	/usr/local/lib/libopencv_superres.dylib\
	/usr/local/lib/libopencv_ts.dylib\
	/usr/local/lib/libopencv_video.dylib\
	/usr/local/lib/libopencv_videostab.dylib
	/bin/rm -f /Users/zhangzhao/Hecate/sources/mapTest/Release/mapTest


PostBuild.mapTest.MinSizeRel:
/Users/zhangzhao/Hecate/sources/mapTest/MinSizeRel/mapTest:\
	/usr/local/lib/libopencv_calib3d.dylib\
	/usr/local/lib/libopencv_contrib.dylib\
	/usr/local/lib/libopencv_core.dylib\
	/usr/local/lib/libopencv_features2d.dylib\
	/usr/local/lib/libopencv_flann.dylib\
	/usr/local/lib/libopencv_gpu.dylib\
	/usr/local/lib/libopencv_highgui.dylib\
	/usr/local/lib/libopencv_imgproc.dylib\
	/usr/local/lib/libopencv_legacy.dylib\
	/usr/local/lib/libopencv_ml.dylib\
	/usr/local/lib/libopencv_nonfree.dylib\
	/usr/local/lib/libopencv_objdetect.dylib\
	/usr/local/lib/libopencv_ocl.dylib\
	/usr/local/lib/libopencv_photo.dylib\
	/usr/local/lib/libopencv_stitching.dylib\
	/usr/local/lib/libopencv_superres.dylib\
	/usr/local/lib/libopencv_ts.dylib\
	/usr/local/lib/libopencv_video.dylib\
	/usr/local/lib/libopencv_videostab.dylib
	/bin/rm -f /Users/zhangzhao/Hecate/sources/mapTest/MinSizeRel/mapTest


PostBuild.mapTest.RelWithDebInfo:
/Users/zhangzhao/Hecate/sources/mapTest/RelWithDebInfo/mapTest:\
	/usr/local/lib/libopencv_calib3d.dylib\
	/usr/local/lib/libopencv_contrib.dylib\
	/usr/local/lib/libopencv_core.dylib\
	/usr/local/lib/libopencv_features2d.dylib\
	/usr/local/lib/libopencv_flann.dylib\
	/usr/local/lib/libopencv_gpu.dylib\
	/usr/local/lib/libopencv_highgui.dylib\
	/usr/local/lib/libopencv_imgproc.dylib\
	/usr/local/lib/libopencv_legacy.dylib\
	/usr/local/lib/libopencv_ml.dylib\
	/usr/local/lib/libopencv_nonfree.dylib\
	/usr/local/lib/libopencv_objdetect.dylib\
	/usr/local/lib/libopencv_ocl.dylib\
	/usr/local/lib/libopencv_photo.dylib\
	/usr/local/lib/libopencv_stitching.dylib\
	/usr/local/lib/libopencv_superres.dylib\
	/usr/local/lib/libopencv_ts.dylib\
	/usr/local/lib/libopencv_video.dylib\
	/usr/local/lib/libopencv_videostab.dylib
	/bin/rm -f /Users/zhangzhao/Hecate/sources/mapTest/RelWithDebInfo/mapTest


