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
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (scale1, _("Saturation"), 1)
  value_range (0.0, 2.4)
  ui_range (0.0, 2.4)
  ui_gamma (1.5)

property_double (scale, _("Sepia strength"), 1.0)
    description(_("Strength of the sepia effect"))
    value_range (0.0, 1.0)


property_double (brightness, _("Brightness"), 0.0)
   description  (_("Amount to increase brightness"))
   value_range  (-0.1, 0.1)
   ui_range     (-0.1, 0.1)

property_double (noisergb, _("Noise meter"), 0.30)
   value_range  (0.0, 1.00)
 
property_double (gaus, _("Blur"), 1.5)
   description (_("mild gaussian blur"))
   value_range (0.0, 3.5)
   ui_range    (0.0, 3.5)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     old
#define GEGL_OP_C_SOURCE old.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *bc, *sat, *sep, *noisergb, *gaus, *bright;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  noisergb = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-rgb",
                                  NULL);


  sat    = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

  sep = gegl_node_new_child (gegl,
                                  "operation", "gegl:sepia",
                                  NULL);

  bright = gegl_node_new_child (gegl,
                                  "operation", "gegl:brightness-contrast",
                                  NULL);

  gaus = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);



  gegl_node_link_many (input, noisergb, gaus, sat, sep, bright, output, NULL);




  gegl_operation_meta_redirect (operation, "scale1", sat, "scale");

  gegl_operation_meta_redirect (operation, "scale", sep, "scale");

  gegl_operation_meta_redirect (operation, "brightness", bright, "brightness");

  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "red");

  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "green");

  gegl_operation_meta_redirect (operation, "noisergb", noisergb, "blue");

  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-x");

  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-y");






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
    "description", _("Simulate a photo from the early 20th century "
                     ""),
    NULL);
}

#endif
