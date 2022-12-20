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

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (scale1, _("Saturation"), -11)
  value_range (-100.0, 20.0)
  ui_range (-100.0, 20.0)
  ui_gamma (1.5)

property_double (scale, _("Sepia strength"), 0.3)
    description(_("Strength of the sepia effect"))
    value_range (0.0, 1.0)


property_double (brightness, _("Brightness"), 0.88)
   description  (_("Amount to increase brightness"))
   value_range  (-30.0, 10.0)
   ui_range     (-30.0, 10.0)

property_double (noisergb, _("Noise meter"), 0.2)
   value_range  (0.0, 1.00)

property_boolean (independent, _("Should RGB noise have color?"), TRUE)
   description (_("Control amount of noise for each RGB channel separately"))
 
property_double (gaus, _("Blur"), 1.5)
   description (_("mild gaussian blur"))
   value_range (0.0, 3.5)
   ui_range    (0.0, 3.5)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")


property_double (shadows, _("Slide to lowest setting to create a vignette"), 0.0)
    description (_("Adjust exposure of shadows"))
    value_range (-100.0, 100.0)

property_double (highlights, _("Highlights"), 0.0)
    description (_("Adjust exposure of highlights"))
    value_range (-70.0, 50.0)

property_double (whitepoint, _("Shadow Highlight White point adjustment"), 0.0)
    description (_("Shift white point"))
    value_range (-10.0, 10.0)
    ui_meta     ("role", "output-extent")

property_double (radius, _("Shadow Highlight Radius"), 100.0)
    description (_("Spatial extent"))
    value_range (0.1, 1500.0)
    ui_range    (0.1, 200.0)
    ui_meta     ("role", "output-extent")

property_double (compress, _("Shadow Hightlight Compress"), 50.0)
    description (_("Compress the effect on shadows/highlights and preserve midtones"))
    value_range (0.0, 100.0)
    ui_meta     ("role", "output-extent")

property_double (shadows_ccorrect, _("Shadows color adjustment"), 100.0)
    description (_("Adjust saturation of shadows"))
    value_range (0.0, 100.0)
    ui_meta     ("role", "output-extent")

property_double (highlights_ccorrect, _("Highlights color adjustment"), 50.0)
    description (_("Adjust saturation of highlights"))
    value_range (0.0, 100.0)
    ui_meta     ("role", "output-extent")




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     old
#define GEGL_OP_C_SOURCE old.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *bc, *sat, *sep, *noisergb, *shadowhighlights, *gaus;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  noisergb = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-rgb",
                                  NULL);


  sat    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  sep = gegl_node_new_child (gegl,
                                  "operation", "gegl:sepia",
                                  NULL);

  gaus = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  shadowhighlights    = gegl_node_new_child (gegl,
                                  "operation", "gegl:shadows-highlights",
                                  NULL);






  gegl_node_link_many (input, noisergb, gaus, shadowhighlights, sat, sep, output, NULL);




  gegl_operation_meta_redirect (operation, "scale1", sat, "chroma");

  gegl_operation_meta_redirect (operation, "scale", sep, "scale");

  gegl_operation_meta_redirect (operation, "brightness", sat, "lightness");

  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "red");

  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "green");

  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "blue");

  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-x");

  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-y");

  gegl_operation_meta_redirect (operation, "independent", noisergb, "independent");

      gegl_operation_meta_redirect (operation, "radius", shadowhighlights, "radius");
      gegl_operation_meta_redirect (operation, "shadows", shadowhighlights, "shadows");
      gegl_operation_meta_redirect (operation, "highlights", shadowhighlights, "highlights");
      gegl_operation_meta_redirect (operation, "whitepoint", shadowhighlights, "whitepoint");
      gegl_operation_meta_redirect (operation, "compress", shadowhighlights, "compress");
      gegl_operation_meta_redirect (operation, "shadows-ccorrect", shadowhighlights, "shadows-ccorrect");
      gegl_operation_meta_redirect (operation, "highlights-ccorrect", shadowhighlights, "highlights-ccorrect");






}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:antique",
    "title",       _("Old Photo filter"),
    "categories",  "Generic",
    "reference-hash", "45ed565h5238500fc2001b2ac",
    "description", _("Simulate a photo from the past by intentionally reducing image quality."
                     ""),
    NULL);
}

#endif
