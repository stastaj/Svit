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
#include "node/solid/triangle.h"
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
#include "light/rectangle.h"
#include "light/background.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <chrono>

using namespace Svit;

void
get_wood_world (World& world,Vector2i& resolution)
{  
  world.camera = new PerspectiveCamera(
                   Point3(0.0, 0.75, -2.0),
                   Vector3(0.0, -0.1, 1.0),
                   Vector3(0.0, 1.0, 0.1),
                   PI_F*0.5f,
                   resolution);
  
  world.scene = new KdTreeGroup();
  
  std::unique_ptr<Texture> checker_texture(new CheckerboardTexture(Vector3(0.5f, 
	    0.5f, 0.5f), Vector3(1.0, 1.0, 1.0), 0.25));
  std::unique_ptr<Material> plane_material(new Phong(
      std::move(checker_texture),25.0f,Vector3(0.3f,0.3f,0.3f)));
  int plane_mat=world.add_material(std::move(plane_material));
	InfinitePlane *plane = new InfinitePlane(Point3(0.0, 0.02, 0.0), 
	    Vector3(0.0, 1.0, 0.0),plane_mat,-1);
	
  WoodPerlinNoiseTexture *wood_texture = new WoodPerlinNoiseTexture(
			Vector3(149.0f/255.0f, 69.0f/255.0f, 53.0f/255.0f), Vector3(237.0f/255.0f,
			201.0f/255.0f, 175.0f/255.0f));
	wood_texture->add_octave(1.0, 3.0);
	std::unique_ptr<Texture> wood_sphere_tex(wood_texture);
  std::unique_ptr<Material> sphere_material(new Phong(
      std::move(wood_sphere_tex),50.0f,Vector3(0.2f,0.2f,0.2f)));
  int sphere_mat=world.add_material(std::move(sphere_material));
  Solid *sphere = new Sphere(Point3(-0.9, 0.35, 0.0), 0.35,sphere_mat,-1);

	world.scene->add(plane);
	world.scene->add(sphere);	

	std::unique_ptr<Light> point_light(new PointLight(Point3(0.0, 1.5, 0.0),
	   Vector3(4.0f, 4.0f, 4.0f)));
  world.add_light(std::move(point_light));
}

void
get_cornell_box_world(World& _world, Vector2i& _resolution, bool point_light, 
                      bool area_light, bool environment_light, bool diffuse){
  _world.camera=new PerspectiveCamera(
              Vector3(-0.0439815f,  0.222539f, -4.12529f),
              Vector3( 0.00688625f,-0.0542161f, 0.998505f),
              Vector3( 3.73896e-4f, 0.998529f,  0.0542148f),
              PI_F * 0.25f, _resolution);
  
  Vector3 glossy5;
  Vector3 glossy7;
  if(! diffuse){
    glossy5=Vector3(0.5f,0.5f,0.5f);
    glossy7=Vector3(0.7f,0.7f,0.7f);
  }
  
  
  Vector3 col1(0.803922f, 0.803922f, 0.803922f);
  Vector3 col2(0.156863f, 0.803922f, 0.172549f);
  Vector3 col3(0.803922f, 0.152941f, 0.152941f);
  Vector3 col4(0.803922f, 0.803922f, 0.152941f);
  Vector3 col5(0.152941f, 0.152941f, 0.803922f);
  
  if(! diffuse){
    col1*=0.5f;
    col2*=0.5f;
    col3*=0.5f;
    col4*=0.5f;
    col5*=0.5f;
  }  
  //white floor 
  std::unique_ptr<Texture> tex1(new ConstantTexture( col1));
  std::unique_ptr<Material> mat1(new Phong(std::move(tex1),90.0f,  glossy5));
  int white=_world.add_material(std::move(mat1));
  //green left wall
  std::unique_ptr<Texture> tex2(new ConstantTexture( col2));
  std::unique_ptr<Material> mat2(new Phong(std::move(tex2),90.0f,  glossy5));
  int green=_world.add_material(std::move(mat2));
  //red right wall
  std::unique_ptr<Texture> tex3(new ConstantTexture( col3));
  std::unique_ptr<Material> mat3(new Phong(std::move(tex3),90.0f,  glossy5));
  int red=_world.add_material(std::move(mat3));
  //yellow sphere
  std::unique_ptr<Texture> tex4(new ConstantTexture( col4));
  std::unique_ptr<Material> mat4(new Phong(std::move(tex4),200.0f,  glossy7));
  int yellow=_world.add_material(std::move(mat4));
  //blue sphere
  std::unique_ptr<Texture> tex5(new ConstantTexture( col5));
  std::unique_ptr<Material> mat5(new Phong(std::move(tex5),600.0f,  glossy7));
  int blue=_world.add_material(std::move(mat5));
  
  Vector3 cb[8] = {
              Vector3(-1.27029f, -1.28002f,  1.30455f),
              Vector3( 1.28975f, -1.28002f,  1.30455f),
              Vector3( 1.28975f,  1.28002f,  1.30455f),
              Vector3(-1.27029f,  1.28002f,  1.30455f),
              Vector3(-1.27029f, -1.28002f, -1.25549f),
              Vector3( 1.28975f, -1.28002f, -1.25549f),
              Vector3( 1.28975f,  1.28002f, -1.25549f),
              Vector3(-1.27029f,  1.28002f, -1.25549f)
          };
  // Floor
  _world.scene = new KdTreeGroup();	
  _world.scene->add(new Triangle(cb[0], cb[4], cb[5], white));
  _world.scene->add(new Triangle(cb[5], cb[1], cb[0], white));
  
  // Left wall
  _world.scene->add(new Triangle(cb[3], cb[7], cb[4], green));
  _world.scene->add(new Triangle(cb[4], cb[0], cb[3], green));

  // Right wall
  _world.scene->add(new Triangle(cb[1], cb[5], cb[6], red));
  _world.scene->add(new Triangle(cb[6], cb[2], cb[1], red));

  // Back wall
  _world.scene->add(new Triangle(cb[0], cb[1], cb[2], white));
  _world.scene->add(new Triangle(cb[2], cb[3], cb[0], white));
      
  // Ceiling || area light
  if(area_light){
    std::unique_ptr<Light> rectangle(new RectangleLight(cb[6], cb[2], cb[7], 
                                 Vector3(1.21f,1.21f,1.21f)));
    _world.add_light( std::move(rectangle) );
    _world.scene->add(new Triangle(cb[2], cb[6], cb[7], white, 0));
    _world.scene->add(new Triangle(cb[7], cb[3], cb[2], white, 0));
  }
  else{
    _world.scene->add(new Triangle(cb[2], cb[6], cb[7], white));
    _world.scene->add(new Triangle(cb[7], cb[3], cb[2], white));
  }
  float smallRadius = 0.5f;
  Vector3 leftWallCenter  = (cb[0] + cb[4]) * (1.f / 2.f) + Vector3(0, smallRadius, 0);
  Vector3 rightWallCenter = (cb[1] + cb[5]) * (1.f / 2.f) + Vector3(0, smallRadius, 0);
  float xlen = rightWallCenter.x - leftWallCenter.x;
  Vector3 leftBallCenter  = leftWallCenter  + Vector3(2.f * xlen / 7.f, 0, 0);
  Vector3 rightBallCenter = rightWallCenter - Vector3(2.f * xlen / 7.f, 0, -xlen/4);

  _world.scene->add(new Sphere(leftBallCenter,  smallRadius, yellow));
  _world.scene->add(new Sphere(rightBallCenter, smallRadius, blue));
  
  
  // Lights
  if(point_light){
    float intensity=50.f/*Watts*/ / (4.f*PI_F);
    std::unique_ptr<Light> point(new PointLight(
                                        Vector3(0.0f, 1.0f, -0.5f),
                                        Vector3(intensity,intensity,intensity)));
    _world.add_light(std::move(point));
  }
  
  if(environment_light){
    std::unique_ptr<Light> background(new BackgroundLight());
    _world.add_light(std::move(background));
  }
  
}

void
get_marble_world (World& _world,Vector2i& resolution)
{
  _world.camera = new PerspectiveCamera(
		Point3(0.0, 24.75, -47.0),
		Vector3(0.0, -0.1, 1.0),
    Vector3(0.0, 1.0, 0.1),
    PI_F*0.5f,
    resolution);
  
  _world.scene = new KdTreeGroup();	
	
	std::unique_ptr<Texture> checker_texture(new CheckerboardTexture(Vector3(0.5f, 
	    0.5f, 0.5f), Vector3(1.0f, 1.0f, 1.0f), 4.25));
  std::unique_ptr<Material> plane_material(new Phong(
      std::move(checker_texture),50.0f,Vector3(0.3f,0.3f,0.3f)));
	int plane_mat=_world.add_material(std::move(plane_material));
  InfinitePlane *plane = new InfinitePlane(Point3(0.0, 0.02, 0.0), 
	    Vector3(0.0, 1.0, 0.0),plane_mat);

	MarblePerlinNoiseTexture *marble_texture = new MarblePerlinNoiseTexture(
			Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 1; i < 1024; i*=2)
		marble_texture->add_octave(1.0f/(float)i, (float)i);

	std::unique_ptr<Texture> marble_sphere_tex(marble_texture);
  std::unique_ptr<Material> marble_sphere_mat(new Phong(
      std::move(marble_sphere_tex),200.0f,Vector3(0.5f,0.5f,0.5f)));
	int marble=_world.add_material(std::move(marble_sphere_mat));
  Sphere *sphere = new Sphere(Point3(5.8, 1.f, 2.0), 2.f, marble);

	_world.scene->add(plane);
	_world.scene->add(sphere);	

  std::unique_ptr<Light> point_light(new PointLight(Point3(-0.5f, 3.5f, 0.0f),
     Vector3(100.0f, 100.0f, 100.0f) ));
  _world.add_light(std::move(point_light));
}

void usage(){
  std::cout<< "Usage: Svit [-i ITERATIONS][-s SCENE_NUMBER][-t TIME][-thr THREADS]"<<std::endl;
  std::cout<< "     ITERATIONS is an unsinged int (number of paths"<<std::endl;
  std::cout<< "     sent through each pixel),"<<std::endl;
  std::cout<< "     SCENE_NUMBER is 0-8"<<std::endl;
  std::cout<< "     TIME is number of seconds to be spent computing,"<<std::endl;
  std::cout<< "     THREADS is number of threads to use."<<std::endl;
}

void parse_params(std::vector<std::string>& _args, Settings& _settings, 
                  World& _world, std::string& _filename)
{
  //get_wood_world(_world,_settings.resolution);
  _settings.max_thread_count = std::thread::hardware_concurrency();
  _settings.iterations= 2;
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
      if(value==0){
        get_cornell_box_world(_world,_settings.resolution,true,false,false,true);
        _filename="0_";
      }
      else if(value==1)
      {
        get_cornell_box_world(_world,_settings.resolution,true,false,false,false);
        _filename="1_";
      }
      else if(value==2){
        get_cornell_box_world(_world,_settings.resolution,false,true,false,true);
        _filename="2_";
      }
      else if(value==3){
        get_cornell_box_world(_world,_settings.resolution,false,true,false,false);
        _filename="3_";
      }
      else if(value==4){
        get_cornell_box_world(_world,_settings.resolution,false,false,true,true);
        _filename="4_";
      }
      else if(value==5){
        get_cornell_box_world(_world,_settings.resolution,false,false,true,false);
        _filename="5_";
      }
      else if(value==6){
        get_wood_world(_world,_settings.resolution);
        _filename="wood_";
      }
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

void SSE_test_normalization(SuperSampling* s){
  int N=500000;
  
  Vector3 w(2.0f,0.5f,-0.4f);
  (~w).dump("normalize_operator");
  w.normalize_fast();
  w.dump("normalize_fast_in_place");
  w=Vector3(2.0f,0.5f,-0.4f);
  w.normalize();
  w.dump("normalize_in_place");
  
  Vector3 v[N];
  for(int i=0;i<N;++i)
    v[i]=s->next_sample();
  
  auto t1 = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;++i){
    v[i].normalize();
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  int elapsed_millisecs = std::chrono::duration_cast
                     <std::chrono::milliseconds>(t2 - t1).count();
  std::cout<<"Normalization in place took "<<elapsed_millisecs<<"ms."<<std::endl;
  
  for(int i=0;i<N;++i)
    v[i]=s->next_sample();
  
  t1 = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;++i){
    v[i].normalize_fast();
  }
  t2 = std::chrono::high_resolution_clock::now();
  elapsed_millisecs = std::chrono::duration_cast
                     <std::chrono::milliseconds>(t2 - t1).count();
  std::cout<<"Fast normalization in place took "<<elapsed_millisecs<<"ms."<<std::endl;
  
  for(int i=0;i<N;++i)
    v[i]=s->next_sample();
  
  t1 = std::chrono::high_resolution_clock::now();
  for(int i=0;i<N;++i){
    v[i]=~v[i];
  }
  t2 = std::chrono::high_resolution_clock::now();
  elapsed_millisecs = std::chrono::duration_cast
                     <std::chrono::milliseconds>(t2 - t1).count();
  std::cout<<"Normalization operator took "<<elapsed_millisecs<<"ms."<<std::endl;
}



int 
main (int argc, char** argv)
{
  SuperSampling* super_sampling=new RandomSuperSampling();
  //SSE_test_normalization(super_sampling);
  
  std::vector<std::string> arguments(argv,argv+argc);
	Settings settings;
  settings.resolution = Vector2i(512, 512);
  World world;
  std::string filename;
  parse_params(arguments,settings,world,filename);

  auto t1 = std::chrono::high_resolution_clock::now();
  world.scene->build_kdtree();
  auto t2 = std::chrono::high_resolution_clock::now();
    int elapsed_millisecs = std::chrono::duration_cast
                       <std::chrono::milliseconds>(t2 - t1).count();
  std::cout<< "Building of kdtree took "<< elapsed_millisecs<< "ms." <<std::endl;
  
  PathTracing engine;
  //SerialRenderer renderer;
  ParallelRenderer renderer;
  
  
  t1 = std::chrono::high_resolution_clock::now();
  
  Image final_image(settings.resolution);
  renderer.render(world, settings, engine,super_sampling,final_image);
  t2 = std::chrono::high_resolution_clock::now();
  elapsed_millisecs = std::chrono::duration_cast
                     <std::chrono::milliseconds>(t2 - t1).count();
  filename+=std::to_string(final_image.iterations)+"i_"+
                       std::to_string(elapsed_millisecs).substr(0,10)+"ms.hdr";
  final_image.write_hdr(filename.data());

  delete super_sampling;
  return 0;
}

