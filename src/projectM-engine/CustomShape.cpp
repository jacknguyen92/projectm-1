/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2004 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "fatal.h"

#include "CustomShape.h"
#include "Eval.h"
#include "Expr.h"
#include "InitCond.h"
#include "Param.h"
#include "PerFrameEqn.h"
#include "Preset.h"
#include "SplayTree.h"

#include "wipemalloc.h"

CustomShape *CustomShape::interface_shape = NULL;

void eval_custom_shape_init_conds(CustomShape * custom_shape);
void load_unspec_init_cond_shape(Param * param);

void destroy_param_db_tree_shape(SplayTree * tree);
void destroy_per_frame_eqn_tree_shape(SplayTree * tree);
void destroy_per_frame_init_eqn_tree_shape(SplayTree * tree);
void destroy_init_cond_tree_shape(SplayTree * tree);

CustomShape::CustomShape( int id ) {

  Param * param;

  this->id = id;
  this->per_frame_count = 0;
  this->per_frame_eqn_string_index = 0;
  this->per_frame_init_eqn_string_index = 0;

  /* Initialize tree data structures */
  this->param_tree = 
       SplayTree::create_splaytree( (int (*)(void*,void*))compare_string, (void* (*)(void*)) copy_string,(void (*)(void*)) free_string);
  
  this->per_frame_eqn_tree = 
       SplayTree::create_splaytree((int (*)(void*, void*))compare_int, (void* (*)(void*))copy_int,(void (*)(void*)) free_int);

  this->init_cond_tree = 
       SplayTree::create_splaytree((int (*)(void*, void*))compare_string, (void* (*)(void*)) copy_string,(void (*)(void*)) free_string);
  
  this->per_frame_init_eqn_tree = 
       SplayTree::create_splaytree((int (*)(void*, void*)) compare_string, (void* (*)(void*))copy_string, (void (*)(void*))free_string);

  /* Start: Load custom shape parameters */
  param = Param::new_param_float("r", P_FLAG_NONE, &this->r, NULL, 1.0, 0.0, 0.5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("g", P_FLAG_NONE, &this->g, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("b", P_FLAG_NONE, &this->b, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("a", P_FLAG_NONE, &this->a, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("border_r", P_FLAG_NONE, &this->border_r, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("border_g", P_FLAG_NONE, &this->border_g, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("border_b", P_FLAG_NONE, &this->border_b, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("border_a", P_FLAG_NONE, &this->border_a, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("r2", P_FLAG_NONE, &this->r2, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("g2", P_FLAG_NONE, &this->g2, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("b2", P_FLAG_NONE, &this->b2, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("a2", P_FLAG_NONE, &this->a2, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("x", P_FLAG_NONE, &this->x, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("y", P_FLAG_NONE, &this->y, NULL, 1.0, 0.0, .5);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_bool("thickOutline", P_FLAG_NONE, &this->thickOutline, 1, 0, 0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_bool("enabled", P_FLAG_NONE, &this->enabled, 1, 0, 0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_int("sides", P_FLAG_NONE, &this->sides, 100, 3, 3);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_bool("additive", P_FLAG_NONE, &this->additive, 1, 0, 0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_bool("textured", P_FLAG_NONE, &this->textured, 1, 0, 0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("rad", P_FLAG_NONE, &this->radius, NULL, MAX_DOUBLE_SIZE, 0, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("ang", P_FLAG_NONE, &this->ang, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("tex_zoom", P_FLAG_NONE, &this->tex_zoom, NULL, MAX_DOUBLE_SIZE, .00000000001, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("tex_ang", P_FLAG_NONE, &this->tex_ang, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t1", P_FLAG_TVAR, &this->t1, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t2", P_FLAG_TVAR, &this->t2, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t3", P_FLAG_TVAR, &this->t3, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t4", P_FLAG_TVAR, &this->t4, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t5", P_FLAG_TVAR, &this->t5, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t6", P_FLAG_TVAR, &this->t6, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t7", P_FLAG_TVAR, &this->t7, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  param = Param::new_param_float("t8", P_FLAG_TVAR, &this->t8, NULL, MAX_DOUBLE_SIZE, -MAX_DOUBLE_SIZE, 0.0);
  if ( param_tree->insert_param( param ) < 0 ) {
    DWRITE( "%s\n", "failed to insert param!" );
  }
  }

void destroy_per_frame_init_eqn_tree_shape(SplayTree * tree) {

  if (!tree)
    return;

  tree->splay_traverse((void (*)(void*))free_init_cond_helper);
  delete tree;

}

void destroy_init_cond_tree_shape(SplayTree * tree) {

  if (!tree)
    return;

  tree->splay_traverse((void (*)(void*))free_init_cond_helper);
  delete tree;

}

void destroy_per_frame_eqn_tree_shape(SplayTree * tree) {


  if (!tree)
    return;

  tree->splay_traverse((void (*)(void*))free_per_frame_eqn_helper);
  delete tree;

}


void destroy_param_db_tree_shape(SplayTree * tree) {

  if (!tree)
    return;

  tree->splay_traverse((void (*)(void*))free_param_helper);
  delete tree;

}

/* Frees a custom shape form object */
CustomShape::~CustomShape() {

  if (param_tree == NULL)
    return;

  destroy_per_frame_eqn_tree_shape(per_frame_eqn_tree);
  destroy_init_cond_tree_shape(init_cond_tree);
  destroy_param_db_tree_shape(param_tree);
  destroy_per_frame_init_eqn_tree_shape(per_frame_init_eqn_tree);
  
  return;

}


CustomShape * CustomShape::find_custom_shape(int id, Preset * preset, int create_flag) {

  CustomShape * custom_shape = NULL;

  if (preset == NULL)
    return NULL;
  
  if ((custom_shape = (CustomShape*)preset->custom_shape_tree->splay_find(&id)) == NULL) {
    
    if (CUSTOM_SHAPE_DEBUG) { printf("find_custom_shape: creating custom shape (id = %d)...", id);fflush(stdout);}
    
    if (create_flag == FALSE) {
      if (CUSTOM_SHAPE_DEBUG) printf("you specified not to (create flag = false), returning null\n");
      return NULL;
    }
    
    if ((custom_shape = new CustomShape(id)) == NULL) {
      if (CUSTOM_SHAPE_DEBUG) printf("failed...out of memory?\n");
      return NULL;
    }
    
    if (CUSTOM_SHAPE_DEBUG) { printf("success.Inserting..."); fflush(stdout);}
    
    if (preset->custom_shape_tree->splay_insert(custom_shape, &custom_shape->id) < 0) {
      if (CUSTOM_SHAPE_DEBUG) printf("failed, probably a duplicated!!\n");
      delete custom_shape;
      return NULL;
    }
    
    if (CUSTOM_SHAPE_DEBUG) printf("done.\n");
  }
  
  return custom_shape;
}

void CustomShape::load_custom_shape_init() {
    load_unspecified_init_conds_shape();
  }

void CustomShape::eval_custom_shape_init_conds() {
  per_frame_init_eqn_tree->splay_traverse((void (*)(void*))eval_init_cond_helper );
}

void CustomShape::load_unspecified_init_conds_shape() {
    interface_shape = this;
    param_tree->splay_traverse((void (*)(void*))load_unspec_init_cond_shape_helper);
    interface_shape = NULL;
  }
