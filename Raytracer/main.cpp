#include <iostream>
#include <vector>

// PNG Encoder
#include "lodepng.h"

// Math
#include "Vector3.h"
#include "Ray.h"

// Scene
#include "Color.h"
#include "Material.h"
#include "Camera.h"
#include "PointLight.h"

// Primitives
#include "Primitive.h"
#include "Plane.h"
#include "Sphere.h"

const unsigned width = 1024;
const unsigned height = 786;
const float aspect = (float)width / (float)height;
const float epsilon = 0.0001f;

std::vector<Primitive*> scene_objects;
PointLight light;

void antialias(std::vector<Color> &pixels, const unsigned &width, const unsigned &height)
{

	std::vector<Color> copy(width*height);
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			copy[width*y + x] = pixels[width*y + x];
		}
	}

	for (unsigned y = 1; y < height - 1; ++y) {
		for (unsigned x = 1; x < width - 1; ++x) {
			float cx = pixels[width*y + x].get_rgb().x() * 0.4f + (
				copy[width*y + x - 1].get_rgb().x() + copy[width*y + x + 1].get_rgb().x() +
				copy[width*(y - 1) + x].get_rgb().x() + copy[width*(y + 1) + x].get_rgb().x()) * 0.25f * 0.6f;

			float cy = pixels[width*y + x].get_rgb().y() * 0.4f + (
				copy[width*y + x - 1].get_rgb().y() + copy[width*y + x + 1].get_rgb().y() +
				copy[width*(y - 1) + x].get_rgb().y() + copy[width*(y + 1) + x].get_rgb().y()) * 0.25f * 0.6f;

			float cz = pixels[width*y + x].get_rgb().z() * 0.4f + (
				copy[width*y + x - 1].get_rgb().z() + copy[width*y + x + 1].get_rgb().z() +
				copy[width*(y - 1) + x].get_rgb().z() + copy[width*(y + 1) + x].get_rgb().z()) * 0.25f * 0.6f;

			pixels[width*y + x] = Color(Vector3(cx, cy, cz), 1.0f);
		}
	}


}

void create_png(std::vector<Color> &pixels, const unsigned &width, const unsigned &height)
{

	std::vector<unsigned char> image;
	image.resize(width * height * 4);

	antialias(pixels, width, height);
	antialias(pixels, width, height);

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			image[width * y * 4 + x * 4 + 0] = pixels[width*y + x].get_rgb().x() * 255;
			image[width * y * 4 + x * 4 + 1] = pixels[width*y + x].get_rgb().y() * 255;
			image[width * y * 4 + x * 4 + 2] = pixels[width*y + x].get_rgb().z() * 255;
			image[width * y * 4 + x * 4 + 3] = pixels[width*y + x].get_alpha() * 255;
		}
	}

	unsigned error = lodepng::encode("test.png", image, width, height);
	if (error) {
		std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}
}

Primitive* raytrace(Ray&, Vector3&, int);

Vector3 compute_color(Ray *ray, Primitive *obj, PointLight light, int depth)
{

	// Ambient Component
	float ambient_intensity = 1.0f;
	Vector3 ambient = Vector3(0.3f, 0.3f, 0.3f) * ambient_intensity;

	// Diffuse Component
	Vector3 intersection = ray->get_origin() + ray->get_direction()*obj->get_intersection(*ray);
	Vector3 obj_to_light = (light.get_position() - intersection).normalize();
	Vector3 obj_normal = obj->get_normal(intersection);
	float dot = obj_to_light.dot(obj_normal);
	dot = (dot + 1.0f) / 2.0f;

	Vector3 diffuse = light.get_color().get_rgb() * obj->get_material().get_diffuse() * dot;
	Vector3 ambient_diffuse = ambient + diffuse;
	Vector3 final_rgb = ambient_diffuse * obj->get_material().get_color().get_rgb();

	// Specular Component
	Vector3 r = obj_normal * obj_to_light.dot(obj_normal) * 2.0f - obj_to_light;
	dot = r.dot(ray->get_direction());
	if (dot > 0.0f) {
		float specular = powf(dot, 20.0f) * obj->get_material().get_specular();
		final_rgb = final_rgb + light.get_color().get_rgb() * specular;
	}

	// Reflection
	Vector3 reflected = ray->get_direction() - (obj_normal * ray->get_direction().dot(obj_normal) * 2);
	Vector3 reflected_color = Vector3(0.0f, 0.0f, 0.0f);
	raytrace(Ray(intersection + reflected*epsilon, reflected), reflected_color, ++depth);

	final_rgb = final_rgb + (reflected_color*obj->get_material().get_color().get_rgb()*obj->get_material().get_reflection());

	return final_rgb;
}

Primitive* raytrace(Ray &ray, Vector3 &rgb, int depth)
{
	static const int MAX_DEPTH = 6;
	if (depth > MAX_DEPTH) return nullptr;

	float dist_to_closest = 999999.0f;
	int closest_object_index = -1;

	for (unsigned i = 0; i < scene_objects.size(); ++i) {
		float dist = scene_objects[i]->get_intersection(ray);
		if (dist >= 0.0f && dist < dist_to_closest) {
			dist_to_closest = dist;
			closest_object_index = i;
		}
	}

	if (closest_object_index == -1) {
		rgb = Vector3(0.2f, 0.2f, 0.2f);
		return nullptr;
	}

	rgb = compute_color(&ray, scene_objects[closest_object_index], light, depth);
	return scene_objects[closest_object_index];
}

int main(int argc, char **argv)
{

	std::vector<Color> pixels;
	pixels.resize(width * height);

	Camera camera;
	Vector3 cam_pos = Vector3(0.0f, 1.5f, -8.0f);
	Vector3 look_at = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 diff = Vector3(look_at.x() - cam_pos.x(), look_at.y() - cam_pos.y(), look_at.z() - cam_pos.z());
	Vector3 cam_fwd = diff.normalize();
	Vector3 cam_right = Vector3(0.0f, 1.0f, 0.0f).cross(cam_fwd).normalize();
	Vector3 cam_up = cam_right.cross(cam_fwd).normalize();
	camera = Camera(cam_pos, cam_right, cam_up, cam_fwd);

	light = PointLight(Vector3(3.0f, 2.0f, 0.0f), Color());

	Sphere s1 = Sphere(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	s1.set_material(Material(Color(Vector3(120.0f / 255.0f, 40.0 / 255.0f, 107.0f / 255.0f), 1.0f), 0.8f, 0.8f, 0.4f));
	Sphere s2 = Sphere(Vector3(2.0f, -0.5f, -2.0f), 0.5f);
	s2.set_material(Material(Color(Vector3(40.0f / 255.0f, 80.0f / 255.0f, 108.0f / 255.0f), 1.0f), 0.8f, 0.4f, 0.3f));
	Plane p1 = Plane(Vector3(0.0f, 1.0f, 0.0f), -1.0f);
	p1.set_material(Material(Color(Vector3(159.0f / 255.0f, 192.0f / 255.0, 0.0f / 255.0f), 1.0f), 0.3f, 0.0f, 0.6f));
	scene_objects.push_back(dynamic_cast<Primitive*>(&s1));
	scene_objects.push_back(dynamic_cast<Primitive*>(&s2));
	scene_objects.push_back(dynamic_cast<Primitive*>(&p1));

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {

			float cam_ray_dx = 0.0f;
			float cam_ray_dy = 0.0f;

			if (aspect > 1.0f) {
				cam_ray_dx = (x + 0.5f) / width*aspect - ((width - height) / (float)height) / 2.0f;
				cam_ray_dy = (height - y + 0.5f) / height;
			}
			else if (aspect < 1.0f) {
				cam_ray_dx = (x + 0.5f) / width;
				cam_ray_dy = (height - y + 0.5f) / height / aspect - ((height - width) / (float)width) / 2.0f;
			}
			else {
				cam_ray_dx = (x + 0.5f) / width;
				cam_ray_dy = (height - y + 0.5f) / height;
			}

			Vector3 cam_ray_origin = camera.get_position();
			Vector3 cam_ray_direction = (camera.get_forward() + (camera.get_right() * (cam_ray_dx - 0.5f) + camera.get_up()*(cam_ray_dy - 0.5f))).normalize();
			Ray cam_ray = Ray(cam_ray_origin, cam_ray_direction);

			Vector3 final_pixel_color = Vector3(0.0f, 0.0f, 0.0f);
			raytrace(cam_ray, final_pixel_color, 1);
			pixels[width*(height - y - 1) + x] = Color(final_pixel_color, 1.0f);
		}
	}

	create_png(pixels, width, height);

	return 0;
}
