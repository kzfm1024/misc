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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_RS2SRC_H_
#define _GST_RS2SRC_H_

#include <gst/base/gstpushsrc.h>

G_BEGIN_DECLS

#define GST_TYPE_RS2SRC   (gst_rs2src_get_type())
#define GST_RS2SRC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RS2SRC,GstRs2src))
#define GST_RS2SRC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RS2SRC,GstRs2srcClass))
#define GST_IS_RS2SRC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RS2SRC))
#define GST_IS_RS2SRC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RS2SRC))

typedef struct _GstRs2src GstRs2src;
typedef struct _GstRs2srcClass GstRs2srcClass;

struct _GstRs2src
{
    // GstBaseSrc base_rs2src;
    GstPushSrc push_rs2src;
};

struct _GstRs2srcClass
{
    // GstBaseSrcClass base_rs2src_class;
    GstPushSrcClass parent_class;    
};

GType gst_rs2src_get_type (void);

G_END_DECLS

#endif
