/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver Old Photo Antique Effect.
 */

/* 2021 GEGL Graph which this plugin is based on.  If you give 
this to Gimp's GEGL Graph filter you can get a static preview of
this filter without installing it.  Fun fact - this is from my
early days of studying GEGL.

noise-rgb 
gaussian-blur std-dev-y=1.3 std-dev-x=1.3
gray
sepia srgb=1 scale=2
saturation scale=1.2 colorspace=Native
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (scale1, _("Color Boost"), -11)
  value_range (-100.0, 20.0)
  ui_range (-100.0, 5.0)
  ui_gamma (1.5)

property_double (scale, _("Sepia strength"), 1.0)
    description(_("Strength of the sepia effect. At 0 there is no sepia effect"))
    value_range (0.0, 1.0)


property_double (lightness, _("Darkness to Lightness"), -10.0)
   description  (_("Lightness control"))
   value_range  (-30.0, 10.0)
   ui_range     (-30.0, 10.0)

property_double (noisergb, _("Noise meter"), 0.2)
   value_range  (0.0, 1.00)
   ui_range  (0.0, 0.5)


property_boolean (independent, _("Should RGB noise have color?"), TRUE)
   description (_("Control amount of noise for each RGB channel separately"))
 
property_double (gaus, _("Blur"), 1.5)
   description (_("Mild gaussian blur to mimic a dated photo's lower quality"))
   value_range (0.0, 3.5)
   ui_range    (0.0, 3.0)
   ui_gamma    (3.0)

property_double (shadows, _("Shadows adjust"), 0.0)
    description (_("Adjust exposure of shadows - on low settings this may create a vignette like effect"))
    value_range (-100.0, 100.0)

property_double (highlights, _("Highlights"), 0.0)
    description (_("Adjust exposure of highlights"))
    value_range (-70.0, 50.0)

property_double (inlow, _("Levels In low light"), 0.0)
    description (_("Levels in low setting to tweak the lighting. Recommended to apply when sepia is present. "))
    value_range (0.0, 1.0)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     old
#define GEGL_OP_C_SOURCE old.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  

  GeglNode *input    = gegl_node_get_input_proxy (gegl, "input");
  GeglNode *output   = gegl_node_get_output_proxy (gegl, "output");


  GeglNode *noisergb = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-rgb",
                                  NULL);


  GeglNode *sat    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  GeglNode *sep = gegl_node_new_child (gegl,
                                  "operation", "gegl:sepia",
                                  NULL);

  GeglNode *opacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  GeglNode *normal = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);

  GeglNode *gaus = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  GeglNode *shadowhighlights    = gegl_node_new_child (gegl,
                                  "operation", "gegl:shadows-highlights", "whitepoint", 0.0, "radius", 0.10,  "compress", 50.0, "shadows-ccorrect", 100.0, "highlights-ccorrect", 50.0,
                                  NULL);

#define mysyntax \
" subtract value=0.1   "\


     GeglNode *graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", mysyntax, 
                                  NULL);

 gegl_node_link_many (input, noisergb, gaus, shadowhighlights, sat, sep, normal, output, NULL);
  gegl_node_connect (normal, "aux", opacity, "output");
 gegl_node_link_many (sep, graph, opacity, NULL);

  gegl_operation_meta_redirect (operation, "scale1", sat, "chroma");
  gegl_operation_meta_redirect (operation, "scale", sep, "scale");
  gegl_operation_meta_redirect (operation, "lightness", sat, "lightness");
  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "red");
  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "green");
  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "blue");
  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-y");
  gegl_operation_meta_redirect (operation, "independent", noisergb, "independent");
  gegl_operation_meta_redirect (operation, "shadows", shadowhighlights, "shadows");
  gegl_operation_meta_redirect (operation, "highlights", shadowhighlights, "highlights");
 gegl_operation_meta_redirect (operation, "inlow", opacity, "value");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:antique",
    "title",       _("Old Photo filter"),
    "reference-hash", "45ed565h5238500fc2001b2ac",
    "description", _("Simulate a photo from the past by intentionally reducing image quality."),

    NULL);
}

#endif
