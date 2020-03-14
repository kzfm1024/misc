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

static GstFlowReturn gst_rs2src_fill (GstPushSrc * src, GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */
static GstStaticPadTemplate gst_rs2src_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (
        "video/x-raw, "
        "format = (string)RGB" ", "
        "width = 640" ", "
        "height = 480" ", "
        "framerate = 30/1"
        )
    );

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
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_rs2src_set_property;
  gobject_class->get_property = gst_rs2src_get_property;
  gobject_class->dispose = gst_rs2src_dispose;
  gobject_class->finalize = gst_rs2src_finalize;

  // push_src_class->create = GST_DEBUG_FUNCPTR (gst_rs2src_create);
  push_src_class->fill = gst_rs2src_fill;
}

static void
gst_rs2src_init (GstRs2src * rs2src)
{
    GST_INFO(__FUNCTION__);

    gst_base_src_set_live (GST_BASE_SRC (rs2src), TRUE);
    
    // GstBaseSrc *base_rs2src = (GstBaseSrc *)&(rs2src->push_rs2src);
    // gst_base_src_set_live (base_rs2src, TRUE);
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
