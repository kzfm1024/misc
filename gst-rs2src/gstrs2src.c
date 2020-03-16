/* GStreamer
 * Copyright (C) 2020 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstrs2src
 *
 * The rs2src element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! rs2src ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>
#include <gst/video/video.h>
#include "gstrs2src.h"

GST_DEBUG_CATEGORY_STATIC (gst_rs2src_debug_category);
#define GST_CAT_DEFAULT gst_rs2src_debug_category

/* prototypes */
static void gst_rs2src_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_rs2src_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_rs2src_dispose (GObject * object);
static void gst_rs2src_finalize (GObject * object);

static gboolean gst_rs2src_start (GstBaseSrc * src);
static gboolean gst_rs2src_stop (GstBaseSrc * src);
static GstCaps *gst_rs2src_get_caps (GstBaseSrc * src, GstCaps * filter);
static gboolean gst_rs2src_set_caps (GstBaseSrc * src, GstCaps * caps);

static GstFlowReturn gst_rs2src_create (GstPushSrc * src, GstBuffer ** buf);

enum
{
  PROP_0
};

/* pad templates */
#if 1
static GstStaticPadTemplate gst_rs2src_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (
        GST_VIDEO_CAPS_MAKE ("{ RGB }")
        )
    );
#else
static GstStaticPadTemplate gst_rs2src_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (
        "video/x-raw, "
        "format = { (string)RGB } " ", "
        "width = 640" ", "
        "height = 480" ", "
        "framerate = 30/1"
        )
    );
#endif

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstRs2src, gst_rs2src, GST_TYPE_BASE_SRC,
    GST_DEBUG_CATEGORY_INIT (gst_rs2src_debug_category, "rs2src", 0,
        "debug category for rs2src element"));

static void
gst_rs2src_class_init (GstRs2srcClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseSrcClass *base_src_class = GST_BASE_SRC_CLASS (klass);
  GstPushSrcClass *push_src_class = GST_PUSH_SRC_CLASS (klass); 

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_rs2src_src_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "RealSense Video Source", "Source/Video",
      "ReelaSense Depth Camera video source",
      "Kazufumi Oyama <Kazufumi.Oyama@sony.com>");

  gobject_class->set_property = gst_rs2src_set_property;
  gobject_class->get_property = gst_rs2src_get_property;
  gobject_class->dispose = gst_rs2src_dispose;
  gobject_class->finalize = gst_rs2src_finalize;

  base_src_class->start = GST_DEBUG_FUNCPTR (gst_rs2src_start);
  base_src_class->stop = GST_DEBUG_FUNCPTR (gst_rs2src_stop);
  base_src_class->get_caps = GST_DEBUG_FUNCPTR (gst_rs2src_get_caps);
  base_src_class->set_caps = GST_DEBUG_FUNCPTR (gst_rs2src_set_caps);

  push_src_class->create = GST_DEBUG_FUNCPTR (gst_rs2src_create);
}

static void
gst_rs2src_init (GstRs2src * src)
{
	/* set source as live (no preroll) */
	gst_base_src_set_live (GST_BASE_SRC (src), TRUE);

	/* override default of BYTES to operate in time mode */
	gst_base_src_set_format (GST_BASE_SRC (src), GST_FORMAT_TIME);

    // src->n_frames
    // src->total_timeouts;
    
    src->framerate = 30.0;
    src->duration = 1000000000.0/src->framerate;  // frame duration in ns
    src->last_frame_time = 0.0;
}

void
gst_rs2src_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstRs2src *rs2src = GST_RS2SRC (object);

  GST_DEBUG_OBJECT (rs2src, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_rs2src_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstRs2src *rs2src = GST_RS2SRC (object);

  GST_DEBUG_OBJECT (rs2src, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_rs2src_dispose (GObject * object)
{
  GstRs2src *rs2src = GST_RS2SRC (object);

  GST_DEBUG_OBJECT (rs2src, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_rs2src_parent_class)->dispose (object);
}

void
gst_rs2src_finalize (GObject * object)
{
  GstRs2src *rs2src = GST_RS2SRC (object);

  GST_DEBUG_OBJECT (rs2src, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_rs2src_parent_class)->finalize (object);
}

static gboolean
gst_rs2src_start (GstBaseSrc * bsrc)
{
    GstRs2src *src = GST_RS2SRC (bsrc);

    src->width = 640; // FIXME
    src->height = 480; // FIXME
    src->val = 0;
    src->dval = 1;

    return TRUE;
}

static gboolean
gst_rs2src_stop (GstBaseSrc * bsrc)
{
	// Start will open the device but not start it, set_caps starts it, stop should stop and close it (as v4l2src)

    GstRs2src *src = GST_RS2SRC (bsrc);
    
#if 0
	GstUEyeSrc *src = GST_UEYE_SRC (bsrc);

	GST_DEBUG_OBJECT (src, "stop");
	UEYEEXECANDCHECK(is_StopLiveVideo(src->hCam, IS_FORCE_VIDEO_STOP));
	UEYEEXECANDCHECK(is_DisableEvent(src->hCam, IS_SET_EVENT_FRAME_RECEIVED));
	UEYEEXECANDCHECK(is_ExitCamera(src->hCam));

	gst_ueye_src_reset (src);
#endif

	return TRUE;
}

static GstCaps *
gst_rs2src_get_caps (GstBaseSrc * bsrc, GstCaps * filter)
{
	GstRs2src *src = GST_RS2SRC (bsrc);
	GstCaps *caps;

    GstVideoInfo vinfo;

    // Create video info 
    gst_video_info_init (&vinfo);

    vinfo.width = src->width;
    vinfo.height = src->height;

   	vinfo.fps_n = 30;  vinfo.fps_d = 1;
    vinfo.interlace_mode = GST_VIDEO_INTERLACE_MODE_PROGRESSIVE;

    vinfo.finfo = gst_video_format_get_info (GST_VIDEO_FORMAT_RGB);

    // cannot do this for variable frame rate
    //src->duration = gst_util_uint64_scale_int (GST_SECOND, vinfo.fps_d, vinfo.fps_n); // NB n and d are wrong way round to invert the fps into a duration.

    caps = gst_video_info_to_caps (&vinfo);

	GST_DEBUG_OBJECT (src, "The caps are %" GST_PTR_FORMAT, caps);

	if (filter) {
		GstCaps *tmp = gst_caps_intersect (caps, filter);
		gst_caps_unref (caps);
		caps = tmp;

		GST_DEBUG_OBJECT (src, "The caps after filtering are %" GST_PTR_FORMAT, caps);
	}

	return caps;
}

static gboolean
gst_rs2src_set_caps (GstBaseSrc * bsrc, GstCaps * caps)
{
	// Start will open the device but not start it, set_caps starts it, stop should stop and close it (as v4l2src)

	GstRs2src *src = GST_RS2SRC (bsrc);
	GstVideoInfo vinfo;
	//GstStructure *s = gst_caps_get_structure (caps, 0);

	GST_DEBUG_OBJECT (src, "The caps being set are %" GST_PTR_FORMAT, caps);

	gst_video_info_from_caps (&vinfo, caps);

	if (GST_VIDEO_INFO_FORMAT (&vinfo) != GST_VIDEO_FORMAT_UNKNOWN) {
		// g_assert (src->hCam != 0);
		//  src->vrm_stride = get_pitch (src->device);  // wait for image to arrive for this
		//src->gst_stride = GST_VIDEO_INFO_COMP_STRIDE (&vinfo, 0);
		//src->nHeight = vinfo.height;
	} else {
		goto unsupported_caps;
	}

	// start freerun/continuous capture

	// UEYEEXECANDCHECK(is_CaptureVideo(src->hCam, IS_FORCE_VIDEO_START));
	// src->acq_started = TRUE;

	return TRUE;

	unsupported_caps:
	GST_ERROR_OBJECT (src, "Unsupported caps: %" GST_PTR_FORMAT, caps);
	return FALSE;
}

static GstFlowReturn
gst_rs2src_create (GstPushSrc * psrc, GstBuffer ** buf)
{
	GstRs2src *src = GST_RS2SRC (psrc);
	GstMapInfo minfo;

	// lock next (raw) image for read access, convert it to the desired
	// format and unlock it again, so that grabbing can go on

	// Wait for the next image to be ready
	// INT timeout = 5000.0/src->framerate;  // 5 times the frame period in ms
	//INT nRet = is_WaitEvent(src->hCam, IS_SET_EVENT_FRAME_RECEIVED, timeout);

	// if(G_LIKELY(nRet == IS_SUCCESS))
    if (1)
	{
		//  successfully returned an image
		// ----------------------------------------------------------

		guint i;

        guint size = src->width * src->height;

		// Create a new buffer for the image
		*buf = gst_buffer_new_and_alloc (size);

		gst_buffer_map (*buf, &minfo, GST_MAP_WRITE);
        {
            for (guint i = 0; i < size; i++)
            {
                minfo.data[i] = src->val;
            }

            if (src->val == 0)
            {
                src->dval = 1;
            }
            if (src->val == 255)
            {
                src->dval = -1;
            }

            src->val += src->dval;
        }
		gst_buffer_unmap (*buf, &minfo);

		// If we do not use gst_base_src_set_do_timestamp() we need to add timestamps manually
		src->last_frame_time += src->duration;   // Get the timestamp for this frame
		if(!gst_base_src_get_do_timestamp(GST_BASE_SRC(psrc))){
			GST_BUFFER_PTS(*buf) = src->last_frame_time;  // convert ms to ns
			GST_BUFFER_DTS(*buf) = src->last_frame_time;  // convert ms to ns
            
		}
		GST_BUFFER_DURATION(*buf) = src->duration;
//		GST_DEBUG_OBJECT(src, "pts, dts: %" GST_TIME_FORMAT ", duration: %d ms", GST_TIME_ARGS (src->last_frame_time), GST_TIME_AS_MSECONDS(src->duration));

		// count frames, and send EOS when required frame number is reached
		GST_BUFFER_OFFSET(*buf) = src->n_frames;  // from videotestsrc
		src->n_frames++;
		GST_BUFFER_OFFSET_END(*buf) = src->n_frames;  // from videotestsrc
		if (psrc->parent.num_buffers>0)  // If we were asked for a specific number of buffers, stop when complete
			if (G_UNLIKELY(src->n_frames >= psrc->parent.num_buffers))
				return GST_FLOW_EOS;

		// see, if we had to drop some frames due to data transfer stalls. if so,
		// output a message
	}
	else
	{
		// did not return an image. why?
		// ----------------------------------------------------------
        /*
		switch(nRet)
		{
		case IS_TIMED_OUT:
			GST_ERROR_OBJECT(src, "is_WaitEvent() timed out.");
			break;
		default:
			GST_ERROR_OBJECT(src, "is_WaitEvent() failed with a generic error.");
			break;
		}
		return GST_FLOW_ERROR;
        */
	}

	return GST_FLOW_OK;
}

#if 0
static GstFlowReturn
gst_rs2src_fill (GstPushSrc * src, GstBuffer * buf)
{
    GstRs2src *rs2src = GST_RS2SRC (src);
    GstMapInfo map;
    gint amount = 0;

    gst_buffer_map (buf, &map, GST_MAP_WRITE);

    {
        static guint8 val = 0;
        amount = 640 * 480 * 3;

        guint8 *data = map.data;
        for (gint i = 0; i < amount; i++)
        {
            data[i] = val;
        }

        val = (val == 255) ? 0 : val + 1;
    }

    gst_buffer_unmap (buf, &map);
    gst_buffer_resize (buf, 0, amount);

    return GST_FLOW_OK;
}
#endif

static gboolean
plugin_init (GstPlugin * plugin)
{
    GST_INFO(__FUNCTION__);

    /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "rs2src", GST_RANK_NONE,
      GST_TYPE_RS2SRC);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.1"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    rs2src,
    "elements for RealSense Depth Camera",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
