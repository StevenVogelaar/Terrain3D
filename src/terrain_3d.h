// Copyright © 2024 Cory Petkovsek, Roope Palmroos, and Contributors.

#ifndef TERRAIN3D_CLASS_H
#define TERRAIN3D_CLASS_H

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/geometry_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "constants.h"
#include "terrain_3d_assets.h"
#include "terrain_3d_data.h"
#include "terrain_3d_editor.h"
#include "terrain_3d_instancer.h"
#include "terrain_3d_material.h"
#include "terrain_3d_storage.h"

using namespace godot;

class Terrain3D : public Node3D {
	GDCLASS(Terrain3D, Node3D);
	CLASS_NAME();

public: // Constants
	enum RegionSize {
		SIZE_64 = 64,
		SIZE_128 = 128,
		SIZE_256 = 256,
		SIZE_512 = 512,
		SIZE_1024 = 1024,
		SIZE_2048 = 2048,
	};

	enum CollisionMode {
		//DYNAMIC_GAME,
		//DYNAMIC_EDITOR,
		FULL_GAME,
		FULL_EDITOR,
	};

private:
	String _version = "0.9.3";
	String _data_directory;
	bool _is_inside_world = false;
	bool _initialized = false;

	// Object references
	Terrain3DData *_data = nullptr;
	Ref<Terrain3DMaterial> _material;
	Ref<Terrain3DAssets> _assets;
	Terrain3DInstancer *_instancer = nullptr;
	Terrain3DEditor *_editor = nullptr;
	EditorPlugin *_plugin = nullptr;
	// Current editor or gameplay camera we are centering the terrain on.
	Camera3D *_camera = nullptr;
	uint64_t _camera_instance_id = 0;
	// X,Z Position of the camera during the previous snapping. Set to max real_t value to force a snap update.
	Vector2 _camera_last_position = V2_MAX;

	// Regions
	RegionSize _region_size = SIZE_256;
	bool _save_16_bit = false;
	real_t _label_distance = 0.f;
	int _label_size = 48;

	// Collision
	RID _static_body;
	StaticBody3D *_debug_static_body = nullptr;
	bool _collision_enabled = true;
	CollisionMode _collision_mode = FULL_GAME;
	uint32_t _collision_layer = 1;
	uint32_t _collision_mask = 1;
	real_t _collision_priority = 1.0f;

	// Meshes
	int _mesh_lods = 7;
	int _mesh_size = 48;
	real_t _vertex_spacing = 1.0f;

	Vector<RID> _meshes;
	struct Instances {
		RID cross;
		Vector<RID> tiles;
		Vector<RID> fillers;
		Vector<RID> trims;
		Vector<RID> seams;
	} _mesh_data;

	// Rendering
	uint32_t _render_layers = 1 | (1 << 31); // Bit 1 and 32 for the cursor
	RenderingServer::ShadowCastingSetting _cast_shadows = RenderingServer::SHADOW_CASTING_SETTING_ON;
	GeometryInstance3D::GIMode _gi_mode = GeometryInstance3D::GI_MODE_STATIC;
	real_t _cull_margin = 0.0f;
	bool _compatibility = false;

	// Mouse cursor
	SubViewport *_mouse_vp = nullptr;
	Camera3D *_mouse_cam = nullptr;
	MeshInstance3D *_mouse_quad = nullptr;
	uint32_t _mouse_layer = 32;

	// Parent containers for child nodes
	Node3D *_label_parent;
	Node3D *_mmi_parent;

	void _initialize();
	void __process(const double p_delta);
	void _grab_camera();

	void _build_containers();
	void _destroy_containers();
	void _destroy_labels();

	void _destroy_instancer();

	bool _is_collision_editor() const { return _collision_mode == FULL_EDITOR; }
	void _build_collision();
	void _update_collision();
	void _destroy_collision();

	void _build_meshes(const int p_mesh_lods, const int p_mesh_size);
	void _update_mesh_instances();
	void _clear_meshes();

	void _setup_mouse_picking();
	void _destroy_mouse_picking();

	void _generate_triangles(PackedVector3Array &p_vertices, PackedVector2Array *p_uvs, const int32_t p_lod,
			const Terrain3DData::HeightFilter p_filter, const bool require_nav, const AABB &p_global_aabb) const;
	void _generate_triangle_pair(PackedVector3Array &p_vertices, PackedVector2Array *p_uvs, const int32_t p_lod,
			const Terrain3DData::HeightFilter p_filter, const bool require_nav, const int32_t x, const int32_t z) const;

public:
	static int debug_level;

	Terrain3D();
	~Terrain3D() {}

	// Terrain
	String get_version() const { return _version; }
	void set_debug_level(const int p_level);
	int get_debug_level() const { return debug_level; }
	void set_data_directory(String p_dir);
	String get_data_directory() const { return (_data == nullptr) ? "" : _data_directory; }

	// Object references
	Terrain3DData *get_data() const { return _data; }
	void set_material(const Ref<Terrain3DMaterial> &p_material);
	Ref<Terrain3DMaterial> get_material() const { return _material; }
	void set_assets(const Ref<Terrain3DAssets> &p_assets);
	Ref<Terrain3DAssets> get_assets() const { return _assets; }
	Terrain3DInstancer *get_instancer() const { return _instancer; }
	Node *get_mmi_parent() const { return _mmi_parent; }
	void set_editor(Terrain3DEditor *p_editor);
	Terrain3DEditor *get_editor() const { return _editor; }
	void set_plugin(EditorPlugin *p_plugin);
	EditorPlugin *get_plugin() const { return _plugin; }
	void set_camera(Camera3D *p_camera);
	Camera3D *get_camera() const { return _camera; }

	// Regions
	void set_region_size(const RegionSize p_size);
	RegionSize get_region_size() const { return _region_size; }
	void change_region_size(const RegionSize p_size) { (_data != nullptr) ? _data->change_region_size(p_size) : void(); }
	void set_save_16_bit(const bool p_enabled);
	bool get_save_16_bit() const { return _save_16_bit; }
	void set_label_distance(const real_t p_distance);
	real_t get_label_distance() const { return _label_distance; }
	void set_label_size(const int p_size);
	int get_label_size() const { return _label_size; }
	void update_region_labels();
	void set_show_grid(const bool p_enabled) { (_material != nullptr) ? _material->set_show_region_grid(p_enabled) : void(); }
	bool get_show_grid() { return (_material != nullptr) ? _material->get_show_region_grid() : false; }

	// Collision
	void set_collision_enabled(const bool p_enabled);
	bool get_collision_enabled() const { return _collision_enabled; }
	void set_collision_mode(const CollisionMode p_mode);
	CollisionMode get_collision_mode() const { return _collision_mode; }
	void set_collision_layer(const uint32_t p_layers);
	uint32_t get_collision_layer() const { return _collision_layer; };
	void set_collision_mask(const uint32_t p_mask);
	uint32_t get_collision_mask() const { return _collision_mask; };
	void set_collision_priority(const real_t p_priority);
	real_t get_collision_priority() const { return _collision_priority; }
	RID get_collision_rid() const;

	// Meshes
	void set_mesh_lods(const int p_count);
	int get_mesh_lods() const { return _mesh_lods; }
	void set_mesh_size(const int p_size);
	int get_mesh_size() const { return _mesh_size; }
	void set_vertex_spacing(const real_t p_spacing);
	real_t get_vertex_spacing() const { return _vertex_spacing; }

	// Rendering
	void set_render_layers(const uint32_t p_layers);
	uint32_t get_render_layers() const { return _render_layers; };
	void set_mouse_layer(const uint32_t p_layer);
	uint32_t get_mouse_layer() const { return _mouse_layer; };
	void set_cast_shadows(const RenderingServer::ShadowCastingSetting p_cast_shadows);
	RenderingServer::ShadowCastingSetting get_cast_shadows() const { return _cast_shadows; };
	void set_gi_mode(const GeometryInstance3D::GIMode p_gi_mode);
	GeometryInstance3D::GIMode get_gi_mode() const { return _gi_mode; }
	void set_cull_margin(const real_t p_margin);
	real_t get_cull_margin() const { return _cull_margin; };
	bool is_compatibility_mode() const { return _compatibility; };

	// Processing
	void snap(const Vector3 &p_cam_pos);
	void update_aabbs();

	// Utility
	Vector3 get_intersection(const Vector3 &p_src_pos, const Vector3 &p_direction);
	Ref<Mesh> bake_mesh(const int p_lod, const Terrain3DData::HeightFilter p_filter = Terrain3DData::HEIGHT_FILTER_NEAREST) const;
	PackedVector3Array generate_nav_mesh_source_geometry(const AABB &p_global_aabb, const bool p_require_nav = true) const;

	// Godot Callbacks
	PackedStringArray _get_configuration_warnings() const override;

protected:
	void _notification(const int p_what);
	static void _bind_methods();

public:
	// DEPRECATED 0.9.2 - Remove 1.0
	void set_texture_list(const Ref<Terrain3DTextureList> &p_texture_list);
	Ref<Terrain3DTextureList> get_texture_list() const { return Ref<Terrain3DTextureList>(); }

	// DEPRECATED 0.9.3 - Remove 1.0
	Ref<Terrain3DStorage> _storage;
	void set_storage(const Ref<Terrain3DStorage> &p_storage);
	Ref<Terrain3DStorage> get_storage() const { return _storage; }
	void split_storage();
};

VARIANT_ENUM_CAST(Terrain3D::RegionSize);
VARIANT_ENUM_CAST(Terrain3D::CollisionMode);

#endif // TERRAIN3D_CLASS_H
