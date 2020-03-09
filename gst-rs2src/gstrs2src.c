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
#include <gst/base/gstbasesrc.h>
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

static GstCaps *gst_rs2src_get_caps (GstBaseSrc * src, GstCaps * filter);
static gboolean gst_rs2src_negotiate (GstBaseSrc * src);
static GstCaps *gst_rs2src_fixate (GstBaseSrc * src, GstCaps * caps);
static gboolean gst_rs2src_set_caps (GstBaseSrc * src, GstCaps * caps);
static gboolean gst_rs2src_decide_allocation (GstBaseSrc * src,
    GstQuery * query);
static gboolean gst_rs2src_start (GstBaseSrc * src);
static gboolean gst_rs2src_stop (GstBaseSrc * src);
static void gst_rs2src_get_times (GstBaseSrc * src, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end);
static gboolean gst_rs2src_get_size (GstBaseSrc * src, guint64 * size);
static gboolean gst_rs2src_is_seekable (GstBaseSrc * src);
static gboolean gst_rs2src_prepare_seek_segment (GstBaseSrc * src,
    GstEvent * seek, GstSegment * segment);
static gboolean gst_rs2src_do_seek (GstBaseSrc * src, GstSegment * segment);
static gboolean gst_rs2src_unlock (GstBaseSrc * src);
static gboolean gst_rs2src_unlock_stop (GstBaseSrc * src);
static gboolean gst_rs2src_query (GstBaseSrc * src, GstQuery * query);
static gboolean gst_rs2src_event (GstBaseSrc * src, GstEvent * event);
static GstFlowReturn gst_rs2src_create (GstBaseSrc * src, guint64 offset,
    guint size, GstBuffer ** buf);
static GstFlowReturn gst_rs2src_alloc (GstBaseSrc * src, guint64 offset,
    guint size, GstBuffer ** buf);
static GstFlowReturn gst_rs2src_fill (GstBaseSrc * src, guint64 offset,
    guint size, GstBuffer * buf);

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
        "video/x-raw, "
        "format = (string)RGB" ", "
        "width = 640" ", "
        "height = 480" ", "
        "framerate = 30"
        )
    );
#else
static GstStaticPadTemplate gst_rs2src_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/unknown")
    );
#endif

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstRs2src, gst_rs2src, GST_TYPE_BASE_SRC,
    GST_DEBUG_CATEGORY_INIT (gst_rs2src_debug_category, "rs2src", 0,
        "debug category for rs2src element"));

static void
gst_rs2src_class_init (GstRs2srcClass * klass)
{
    GST_INFO(__FUNCTION__);
    
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseSrcClass *base_src_class = GST_BASE_SRC_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_rs2src_src_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_rs2src_set_property;
  gobject_class->get_property = gst_rs2src_get_property;
  gobject_class->dispose = gst_rs2src_dispose;
  gobject_class->finalize = gst_rs2src_finalize;
/*
  base_src_class->get_caps = GST_DEBUG_FUNCPTR (gst_rs2src_get_caps);
  base_src_class->negotiate = GST_DEBUG_FUNCPTR (gst_rs2src_negotiate);
  base_src_class->fixate = GST_DEBUG_FUNCPTR (gst_rs2src_fixate);
  base_src_class->set_caps = GST_DEBUG_FUNCPTR (gst_rs2src_set_caps);
  base_src_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_rs2src_decide_allocation);
  base_src_class->start = GST_DEBUG_FUNCPTR (gst_rs2src_start);
  base_src_class->stop = GST_DEBUG_FUNCPTR (gst_rs2src_stop);
  base_src_class->get_times = GST_DEBUG_FUNCPTR (gst_rs2src_get_times);
  base_src_class->get_size = GST_DEBUG_FUNCPTR (gst_rs2src_get_size);
  base_src_class->is_seekable = GST_DEBUG_FUNCPTR (gst_rs2src_is_seekable);
  base_src_class->prepare_seek_segment =
      GST_DEBUG_FUNCPTR (gst_rs2src_prepare_seek_segment);
  base_src_class->do_seek = GST_DEBUG_FUNCPTR (gst_rs2src_do_seek);
  base_src_class->unlock = GST_DEBUG_FUNCPTR (gst_rs2src_unlock);
  base_src_class->unlock_stop = GST_DEBUG_FUNCPTR (gst_rs2src_unlock_stop);
  base_src_class->query = GST_DEBUG_FUNCPTR (gst_rs2src_query);
  base_src_class->event = GST_DEBUG_FUNCPTR (gst_rs2src_event);
  base_src_class->create = GST_DEBUG_FUNCPTR (gst_rs2src_create);
  base_src_class->alloc = GST_DEBUG_FUNCPTR (gst_rs2src_alloc);
*/
  base_src_class->fill = GST_DEBUG_FUNCPTR (gst_rs2src_fill);
}

static void
gst_rs2src_init (GstRs2src * rs2src)
{
    GST_INFO(__FUNCTION__);

    /*
    rs2src->srcpad = gst_pad_new_from_static_template (&gst_rs2src_src_template, "src");
    gst_element_add_pad (GST_ELEMENT (rs2src), rs2src->srcpad);

    rs2src->silent = FALSE;
    */
}

void
gst_rs2src_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
    GST_INFO(__FUNCTION__);
        
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
    GST_INFO(__FUNCTION__);

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

/* get caps from subclass */
static GstCaps *
gst_rs2src_get_caps (GstBaseSrc * src, GstCaps * filter)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "get_caps");

  return NULL;
}

/* decide on caps */
static gboolean
gst_rs2src_negotiate (GstBaseSrc * src)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "negotiate");

  return TRUE;
}

/* called if, in negotiation, caps need fixating */
static GstCaps *
gst_rs2src_fixate (GstBaseSrc * src, GstCaps * caps)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "fixate");

  return NULL;
}

/* notify the subclass of new caps */
static gboolean
gst_rs2src_set_caps (GstBaseSrc * src, GstCaps * caps)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "set_caps");

  return TRUE;
}

/* setup allocation query */
static gboolean
gst_rs2src_decide_allocation (GstBaseSrc * src, GstQuery * query)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "decide_allocation");

  return TRUE;
}

/* start and stop processing, ideal for opening/closing the resource */
static gboolean
gst_rs2src_start (GstBaseSrc * src)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "start");

  return TRUE;
}

static gboolean
gst_rs2src_stop (GstBaseSrc * src)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "stop");

  return TRUE;
}

/* given a buffer, return start and stop time when it should be pushed
 * out. The base class will sync on the clock using these times. */
static void
gst_rs2src_get_times (GstBaseSrc * src, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "get_times");

}

/* get the total size of the resource in bytes */
static gboolean
gst_rs2src_get_size (GstBaseSrc * src, guint64 * size)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "get_size");

  return TRUE;
}

/* check if the resource is seekable */
static gboolean
gst_rs2src_is_seekable (GstBaseSrc * src)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "is_seekable");

  return TRUE;
}

/* Prepare the segment on which to perform do_seek(), converting to the
 * current basesrc format. */
static gboolean
gst_rs2src_prepare_seek_segment (GstBaseSrc * src, GstEvent * seek,
    GstSegment * segment)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "prepare_seek_segment");

  return TRUE;
}

/* notify subclasses of a seek */
static gboolean
gst_rs2src_do_seek (GstBaseSrc * src, GstSegment * segment)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "do_seek");

  return TRUE;
}

/* unlock any pending access to the resource. subclasses should unlock
 * any function ASAP. */
static gboolean
gst_rs2src_unlock (GstBaseSrc * src)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "unlock");

  return TRUE;
}

/* Clear any pending unlock request, as we succeeded in unlocking */
static gboolean
gst_rs2src_unlock_stop (GstBaseSrc * src)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "unlock_stop");

  return TRUE;
}

/* notify subclasses of a query */
static gboolean
gst_rs2src_query (GstBaseSrc * src, GstQuery * query)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "query");

  return TRUE;
}

/* notify subclasses of an event */
static gboolean
gst_rs2src_event (GstBaseSrc * src, GstEvent * event)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "event");

  return TRUE;
}

/* ask the subclass to create a buffer with offset and size, the default
 * implementation will call alloc and fill. */
static GstFlowReturn
gst_rs2src_create (GstBaseSrc * src, guint64 offset, guint size,
    GstBuffer ** buf)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "create");

  return GST_FLOW_OK;
}

/* ask the subclass to allocate an output buffer. The default implementation
 * will use the negotiated allocator. */
static GstFlowReturn
gst_rs2src_alloc (GstBaseSrc * src, guint64 offset, guint size,
    GstBuffer ** buf)
{
  GstRs2src *rs2src = GST_RS2SRC (src);

  GST_DEBUG_OBJECT (rs2src, "alloc");

  return GST_FLOW_OK;
}

/* ask the subclass to fill the buffer with data from offset and size */
static GstFlowReturn
gst_rs2src_fill (GstBaseSrc * src, guint64 offset, guint size, GstBuffer * buf)
{
    static guint32 i = 0; // FIXME

  GstRs2src *rs2src = GST_RS2SRC (src);
  GstMapInfo info;


  GST_DEBUG_OBJECT (rs2src, "fill");

  // if (rs2src->silent == FALSE)
  {
      g_print("%s: i %d offset %ld size %d\n", __FUNCTION__, i, offset, size);
  }

  if (size < sizeof(i))
  {
      GST_ERROR_OBJECT(rs2src, "gst_rs2src_fill: size too short");
      return GST_FLOW_ERROR;
  }

  if (!gst_buffer_map (buf, &info, GST_MAP_WRITE)) return GST_FLOW_ERROR;

  guint8 *data = info.data;  
  memcpy(data, &i, sizeof(i));
  i++;

  gst_buffer_unmap (buf, &info);
  gst_buffer_resize(buf, 0, sizeof(i));


  if (i == 30)
  {
      i = 0;
      return GST_FLOW_EOS;
  }
  
  return GST_FLOW_OK;
}

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
