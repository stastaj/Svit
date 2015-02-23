#include "camera/perspective.h"
#include "geom/rect.h"
#include "geom/vector.h"
#include "geom/point.h"
#include "engine/engine.h"
#include "engine/path_tracing.h"
#include "image/image.h"
#include "node/group/kdtree_group.h"
#include "node/group/simple_group.h"
#include "node/solid/sphere.h"
#include "node/solid/infinite_plane.h"
#include "node/solid/disc.h"
#include "renderer/settings.h"
#include "renderer/serial/serial.h"
#include "renderer/parallel/parallel.h"
#include "supersampling/random.h"
#include "world/world.h"
#include "material/material.h"
#include "material/phong.h"
#include "texture/constant.h"
#include "texture/checkerboard.h"
#include "texture/perlin_noise.h"
#include "texture/wood_perlin_noise.h"
#include "texture/marble_perlin_noise.h"
#include "light/point.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <chrono>

using namespace Svit;

void
get_wood_world (World& world,Vector2i& resolution)
{
	KdTreeGroup *scene = new KdTreeGroup();

	InfinitePlane *plane = new InfinitePlane(Point3(0.0, 0.02, 0.0), 
	    Vector3(0.0, 1.0, 0.0));
	std::unique_ptr<Texture> checker_texture(new CheckerboardTexture(Vector3(0.5f, 
	    0.5f, 0.5f), Vector3(1.0, 1.0, 1.0), 0.25));
  std::unique_ptr<Material> plane_material(new Phong(
      std::move(checker_texture),5.0f,Vector3(0.f,0.f,0.f)));
	plane->set_material(std::move(plane_material));

  Solid *sphere = new Sphere(Point3(-0.9, 0.35, 0.0), 0.35);
	WoodPerlinNoiseTexture *wood_texture = new WoodPerlinNoiseTexture(
			Vector3(149.0f/255.0f, 69.0f/255.0f, 53.0f/255.0f), Vector3(237.0f/255.0f,
			201.0f/255.0f, 175.0f/255.0f));
	wood_texture->add_octave(1.0, 3.0);
	std::unique_ptr<Texture> wood_sphere_tex(wood_texture);
  std::unique_ptr<Material> sphere_material(new Phong(
      std::move(wood_sphere_tex),5.0f,Vector3(0.2f,0.2f,0.2f)));
	sphere->set_material(std::move(sphere_material));

	scene->add(plane);
	scene->add(sphere);
  
  auto t1 = std::chrono::high_resolution_clock::now();
  scene->build_kdtree();
  auto t2 = std::chrono::high_resolution_clock::now();
    int elapsed_millisecs = std::chrono::duration_cast
                       <std::chrono::milliseconds>(t2 - t1).count();
  std::cout<< "Building of kdtree took "<< elapsed_millisecs<< "ms." <<std::endl;
  
	PerspectiveCamera *camera = new PerspectiveCamera(
		Point3(0.0, 0.75, -2.0),
		Vector3(0.0, -0.1, 1.0),
    Vector3(0.0, 1.0, 0.1),
    PI_F*0.5f,
    resolution);

	//World world;
	world.scene = scene;
	world.camera = camera;

	std::unique_ptr<Light> point_light(new PointLight(Point3(0.0, 1.5, 0.0),
	   Vector3(1.0f, 1.0f, 1.0f) * 2.0f));
	world.add_light(std::move(point_light));
}

void
get_cornell_box_world(World& _world, Vector2i& _resolution){
  Vector3 cb[8] = {
              Vector3(-1.27029f,  1.30455f, -1.28002f),
              Vector3( 1.28975f,  1.30455f, -1.28002f),
              Vector3( 1.28975f,  1.30455f,  1.28002f),
              Vector3(-1.27029f,  1.30455f,  1.28002f),
              Vector3(-1.27029f, -1.25549f, -1.28002f),
              Vector3( 1.28975f, -1.25549f, -1.28002f),
              Vector3( 1.28975f, -1.25549f,  1.28002f),
              Vector3(-1.27029f, -1.25549f,  1.28002f)
          };
  
  
}

World
get_marble_world (Vector2i& resolution)
{
	SimpleGroup *scene = new SimpleGroup();

	InfinitePlane *plane = new InfinitePlane(Point3(0.0, 0.02, 0.0), 
	    Vector3(0.0, 1.0, 0.0));
	std::unique_ptr<Texture> checker_texture(new CheckerboardTexture(Vector3(0.5f, 
	    0.5f, 0.5f), Vector3(1.0, 1.0, 1.0), 4.25));
  std::unique_ptr<Material> plane_material(new Phong(
      std::move(checker_texture),50.0f,Vector3(0.0f,0.0f,0.0f)));
	plane->set_material(std::move(plane_material));

	MarblePerlinNoiseTexture *marble_texture = new MarblePerlinNoiseTexture(
			Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 1; i < 1024; i*=2)
		marble_texture->add_octave(1.0f/(float)i, (float)i);

	std::unique_ptr<Texture> marble_sphere_tex(marble_texture);
  std::unique_ptr<Material> marble_sphere_mat(new Phong(
      std::move(marble_sphere_tex),50.0f,Vector3(0.0f,0.0f,0.0f)));
	Sphere *sphere = new Sphere(Point3(12.8, 15.35, 2.0), 15.35);
	sphere->set_material(std::move(marble_sphere_mat));

	scene->add(plane);
	scene->add(sphere);

	PerspectiveCamera *camera = new PerspectiveCamera(
		Point3(0.0, 24.75, -47.0),
		Vector3(0.0, -0.1, 1.0),
    Vector3(0.0, 1.0, 0.1),
    PI_F*0.5f,
    resolution);

	World world;
	world.scene = scene;
	world.camera = camera;

  std::unique_ptr<Light> point_light(new PointLight(Point3(0.0, 1.5, 0.0),
     Vector3(1.0f, 1.0f, 1.0f) * 1.0f));
	return world;
}

void usage(){
  std::cout<< "Usage: Svit [-i ITERATIONS][-s SCENE_NUMBER][-t TIME][-thr THREADS]"<<std::endl;
  std::cout<< "     ITERATIONS is an unsinged int (number of paths"<<std::endl;
  std::cout<< "     sent through each pixel),"<<std::endl;
  std::cout<< "     SCENE_NUMBER is 0-2"<<std::endl;
  std::cout<< "     TIME is number of seconds to be spent computing,"<<std::endl;
  std::cout<< "     THREADS is number of threads to use."<<std::endl;
}

void parse_params(std::vector<std::string>& _args, Settings& _settings, 
                  World& _world)
{
  get_wood_world(_world,_settings.resolution);
  _settings.max_thread_count = std::thread::hardware_concurrency();
  _settings.iterations= 10;
  _settings.time=0;
  for (auto it = ++ begin (_args); it != end (_args); ++it) {
    if(*it=="-i"){
      if(++it==end(_args)){
        usage();
        std::exit(1);
      }
      std::istringstream reader(*it);
      unsigned int value;
      reader >> value;
      _settings.iterations=value;
    }
    else if(*it=="-s"){
      if(++it==end(_args)){
        usage();
        std::exit(1);
      }
      std::istringstream reader(*it);
      unsigned int value;
      reader >> value;
      if(value==0)
      {}//get_wood_world(_world,_settings.resolution);
      else{
        std::cout<<"Unknown scene number. "<<std::endl;
        usage();
      }
    }
    else if(*it=="-thr"){
      if(++it==end(_args)){
        usage();
        std::exit(1);
      }
      std::istringstream reader(*it);
      unsigned int value;
      reader >> value;
      _settings.max_thread_count=value;
    }
  }
}



int 
main (int argc, char** argv)
{
  std::vector<std::string> arguments(argv,argv+argc);
	Settings settings;
  settings.resolution = Vector2i(1280, 720);
  World world;
  
  parse_params(arguments,settings,world);

  PathTracing engine;
  ParallelRenderer renderer;
  SuperSampling* super_sampling=new RandomSuperSampling();
  
  auto t1 = std::chrono::high_resolution_clock::now();
  
  Image final_image(settings.resolution);
  renderer.render(world, settings, engine,super_sampling,final_image);
  auto t2 = std::chrono::high_resolution_clock::now();
  int elapsed_millisecs = std::chrono::duration_cast
                     <std::chrono::milliseconds>(t2 - t1).count();
  std::string filename="wood_"+std::to_string(final_image.iterations)+"i_"+
                       std::to_string(elapsed_millisecs).substr(0,10)+"ms.png";
  final_image.write(filename.data());

  delete super_sampling;
  return 0;
}

