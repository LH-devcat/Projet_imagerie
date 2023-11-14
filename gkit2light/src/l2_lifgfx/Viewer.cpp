
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "draw.h"        // pour dessiner du point de vue d'une camera
#include "Viewer.h"

using namespace std;


Viewer::Viewer() : App(1024, 768), mb_cullface(true), mb_wireframe(false), b_draw_grid(true), b_draw_axe(true), b_draw_animation(false)
{
}


void Viewer::help()
{
    printf("HELP:\n");
    printf("\th: help\n");
    printf("\tc: (des)active GL_CULL_FACE\n");
    printf("\tw: (des)active wireframe\n");
    printf("\ta: (des)active l'affichage de l'axe\n");
    printf("\tg: (des)active l'affichage de la grille\n");
    printf("\tz: (des)active l'affichage de la courbe d'animation\n");
    printf("\tfleches/pageUp/pageDown: bouge la camera\n");
    printf("\tCtrl+fleche/pageUp/pageDown: bouge la source de lumiere\n");
    printf("\tSouris+bouton gauche: rotation\n");
    printf("\tSouris mouvement vertical+bouton droit: (de)zoom\n");
}

int Viewer::init()
{
    cout<<"==>l2_lifgfx/Viewer"<<endl;
    // etat par defaut openGL
    glClearColor(0.5f, 0.5f, 0.9f, 1);
    glClearDepthf(1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    if (mb_cullface)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);        // good for debug
    glEnable(GL_TEXTURE_2D);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glAlphaFunc(GL_GREATER, 0.5);
    //glEnable(GL_ALPHA_TEST);

    m_anim.init( smart_path("data/animation/anim1.ani") );

    m_camera.lookat( Point(0,0,0), 30 );
    gl.light( Point(0, 20, 20), White() );

    init_axe();
    init_grid();
    init_cube();
    init_quad();
    init_sphere();
    init_terrain();
    init_cubemap();
    init_arbre();

//    m_tex_mur = read_texture(0, smart_path("data/mur.png")) ;
 //   m_tex_pacman = read_texture(0, smart_path("data/pacman.png")) ;;
 //   m_tex_fantome = read_texture(0, smart_path("data/fantome.png")) ;;
  //  m_tex_pastille = read_texture(0, smart_path("data/pastille.png")) ;;

    return 0;
}



void Viewer::init_arbre(){
   m_arbre_texture = read_texture(0,smart_path("/gkit2light/data/billboard"));





}

void Viewer::init_axe()
{
    m_axe = Mesh(GL_LINES);
    m_axe.color( Color(1, 0, 0));
    m_axe.vertex( 0,  0, 0);
    m_axe.vertex( 1,  0, 0);

    m_axe.color( Color(0, 1, 0));
    m_axe.vertex( 0,  0, 0);
    m_axe.vertex( 0,  1, 0);

    m_axe.color( Color( 0, 0, 1));
    m_axe.vertex( 0,  0, 0);
    m_axe.vertex( 0,  0, 1);
}


void Viewer::init_grid()
{
    m_grid = Mesh(GL_LINES);

    m_grid.color( Color(1, 1, 1));
    int i,j;
    for(i=-5;i<=5;++i)
        for(j=-5;j<=5;++j)
        {
            m_grid.vertex( -5, 0, j);
            m_grid.vertex( 5, 0,  j);

            m_grid.vertex( i, 0, -5);
            m_grid.vertex( i, 0, 5);

        }
}

void Viewer::init_cubemap()
{
    //                          0           1           2       3           4           5       6           7
    static float pt[8][3] = { {-1,-1,-1}, {1,-1,-1}, {1,-1,1}, {-1,-1,1}, {-1,1,-1}, {1,1,-1}, {1,1,1}, {-1,1,1} };
    static int f[6][4] = {    {0,1,2,3}, {5,4,7,6}, {2,1,5,6}, {0,3,7,4}, {3,2,6,7}, {1,0,4,5} };
    static float n[6][3] = { {0,1,0}, {0,-1,0}, {-1,0,0}, {1,0,0}, {0,0,-1}, {0,0,1} };
    int i,j;

    m_cubemap = Mesh(GL_TRIANGLE_STRIP);
    m_cubemap.color( Color(1, 1, 1) );

    m_cubemap_texture = read_texture(0, smart_path("data/cubemap/skybox.png")) ;

    for (i=0;i<6;i++)
    {
        m_cubemap.normal(  n[i][0], n[i][1], n[i][2] );

        m_cubemap.texcoord(1,1);
        m_cubemap.vertex( pt[ f[i][2] ][0], pt[ f[i][2] ][1], pt[ f[i][2] ][2] );

        m_cubemap.texcoord( 1,0);
        m_cubemap.vertex( pt[ f[i][1] ][0], pt[ f[i][1] ][1], pt[ f[i][1] ][2] );

        m_cubemap.texcoord(0,1);
        m_cubemap.vertex(pt[ f[i][3] ][0], pt[ f[i][3] ][1], pt[ f[i][3] ][2] );



        m_cubemap.texcoord( 0,0 );
        m_cubemap.vertex( pt[ f[i][0] ][0], pt[ f[i][0] ][1], pt[ f[i][0] ][2] );







        m_cubemap.restart_strip();
    }
}


void Viewer::init_cube()
{
    //                          0           1           2       3           4           5       6           7
    static float pt[8][3] = { {-1,-1,-1}, {1,-1,-1}, {1,-1,1}, {-1,-1,1}, {-1,1,-1}, {1,1,-1}, {1,1,1}, {-1,1,1} };
    static int f[6][4] = {    {0,1,2,3}, {5,4,7,6}, {2,1,5,6}, {0,3,7,4}, {3,2,6,7}, {1,0,4,5} };
    static float n[6][3] = { {0,-1,0}, {0,1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1} };
    int i,j;

    m_cube = Mesh(GL_TRIANGLE_STRIP);
    m_cube.color( Color(1, 1, 1) );

    m_cube_texture = read_texture(0, smart_path("data/debug2x2red.png")) ;

    for (i=0;i<6;i++)
    {
        m_cube.normal(  n[i][0], n[i][1], n[i][2] );

        m_cube.texcoord( 0,0 );
        m_cube.vertex( pt[ f[i][0] ][0], pt[ f[i][0] ][1], pt[ f[i][0] ][2] );

        m_cube.texcoord( 1,0);
        m_cube.vertex( pt[ f[i][1] ][0], pt[ f[i][1] ][1], pt[ f[i][1] ][2] );

        m_cube.texcoord(0,1);
        m_cube.vertex(pt[ f[i][3] ][0], pt[ f[i][3] ][1], pt[ f[i][3] ][2] );

        m_cube.texcoord(1,1);
        m_cube.vertex( pt[ f[i][2] ][0], pt[ f[i][2] ][1], pt[ f[i][2] ][2] );

        m_cube.restart_strip();
    }
}

void Viewer::init_sphere()
{
    int divBeta = 10;
    int divAlpha = 10;
    Point actualPoint, nextPoint, nextVerticalPoint, nextHorizontalPoint;
    Vector normale, vecteurHorizontal, vecteurVertical;

    float alpha, alpha2, beta, beta2;

    m_sphere = Mesh(GL_TRIANGLE_STRIP);
    m_sphere.color(Color(1, 1, 1));

    for(int i = 0; i < divAlpha; i++)
    {
      alpha = (float)-0.5*M_PI+(i * M_PI)/divAlpha;
      alpha2 = (float)-0.5*M_PI + ((i+1)*M_PI)/divAlpha;
      for(int j = 0; j < divBeta; j++) {

        beta = (float)j*2.0*M_PI/divBeta;
        beta2 = (float)(j-1)*2.0*M_PI/divBeta;

        nextHorizontalPoint = Point(cos(alpha2)*cos(beta), sin(alpha2), cos(alpha2)*sin(beta));
        nextVerticalPoint = Point(cos(alpha)*cos(beta2), sin(alpha), cos(alpha)*sin(beta2));

        actualPoint = Point(cos(alpha)*cos(beta), sin(alpha), cos(alpha)*sin(beta));
        nextPoint = Point(cos(alpha)*cos(beta2), sin(alpha), cos(alpha)*sin(beta2));


        vecteurHorizontal = Vector(nextPoint , nextVerticalPoint );
        vecteurVertical = Vector(actualPoint, nextVerticalPoint);

        normale = cross(vecteurHorizontal, vecteurVertical);
        cout << "Point : " << actualPoint.x << " " << actualPoint.y << " " << actualPoint.z << endl;
        cout << "Normale : " << normale.x << " " << normale.y << " " << normale.z << endl;
        // normale = normalize(normale);
        m_sphere.normal(normale);
        m_sphere.vertex(actualPoint);
      }
      m_sphere.normal(normale);
      m_sphere.vertex(nextVerticalPoint);
    }

    m_sphere.restart_strip();

}

void Viewer::init_terrain() {
    m_terrain = Mesh(GL_TRIANGLE_STRIP);
    m_terrain.color(Color(1,1,1));

    m_terrain_texture = read_texture(0, smart_path("data/terrain/terrain_texture.png") );
    m_terrain_height_map = read_image(smart_path("data/terrain/terrain.png") );
    cout << "Texture : "<< m_terrain_texture << endl;

    int dimx = 20;
    int dimz = 20;
    float pointx, pointz, pointx2, pointz2;

    for(int i = 0; i < dimx; ++i) {
            pointx = (float)i/dimx;
            pointx2 = (float)(i+1)/dimx;
        for(int j = 0; j < dimz; ++j) {
            pointz = (float)j/dimz;
            pointz2 = (float)(j+1)/dimz;
            if(pointz2 >= 1) {
                pointz2 = pointz;
            }
            if(pointx2 >= 1) {
                pointx2 = pointx;
            }

            m_terrain.normal(0,1,0);

            m_terrain.texcoord(pointx2, pointz2);
            m_terrain.vertex(Point(pointx2, (float)m_terrain_height_map(pointx2*m_terrain_height_map.width(), pointz2*m_terrain_height_map.height()).r, pointz2));
            cout << "R pour ( " << (int)pointx2*m_terrain_height_map.width() << " , " << (int)pointz2*m_terrain_height_map.height() << " )" << (float)m_terrain_height_map(pointx2*m_terrain_height_map.width(), pointz2*m_terrain_height_map.height()).r << endl;

            m_terrain.texcoord(pointx, pointz2);
            m_terrain.vertex(Point(pointx, (float)m_terrain_height_map(pointx*m_terrain_height_map.width(), pointz2*m_terrain_height_map.height()).r, pointz2));

            m_terrain.texcoord(pointx2, pointz);
            m_terrain.vertex(Point(pointx2, (float)m_terrain_height_map(pointx2*m_terrain_height_map.width(), pointz*m_terrain_height_map.height()).r, pointz));

            m_terrain.texcoord(pointx, pointz);
            m_terrain.vertex(Point(pointx, (float)m_terrain_height_map(pointx*m_terrain_height_map.width(), pointz*m_terrain_height_map.height()).r, pointz));






        }
        //m_terrain.vertex(Point(pointx, 0, pointz));
        m_terrain.restart_strip();
    }


}

void Viewer::init_quad()
{
    m_quad = Mesh(GL_TRIANGLE_STRIP);
    m_quad.color( Color(1, 1, 1));

    m_quad_texture = read_texture(0, smart_path("data/papillon.png") );

    m_quad.normal(  0, 0, 1 );

    m_quad.texcoord(0,0 );
    m_quad.vertex(-1, -1, 0 );

    m_quad.texcoord(1,0);
    m_quad.vertex(  1, -1, 0 );

    m_quad.texcoord(0,1);
    m_quad.vertex( -1, 1, 0 );

    m_quad.texcoord( 1,1);
    m_quad.vertex(  1,  1, 0 );
}


int Viewer::render( )
{
    // Efface l'ecran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Deplace la camera, lumiere, etc.
    manageCameraLight();

    // donne notre camera au shader
    gl.camera(m_camera);

    gl.model(Translation(0,10,0)*Scale(3,3,3));
    gl.draw(m_cube);

    gl.model(Translation(5,5,0)*Scale(3,3,3));
    gl.draw(m_sphere);

    gl.texture(m_terrain_texture);
    gl.model(Translation(-25,0,-25)*Scale(50,5,50));
    gl.draw(m_terrain);

    gl.texture(m_cubemap_texture);
    gl.model(Scale(50,50,50));
    gl.draw(m_cubemap);

    // papillon
    gl.texture(m_quad_texture);
    gl.model( Tquad );
    gl.draw(m_quad);



    return 1;
}



int Viewer::update( const float time, const float delta )
{
    Tquad = Translation( 3, 5, 0 ) * Rotation( Vector(0,0,1), 0.1f*time);


    return 1;
}





void Viewer::arbre(const Transform& T){

    for(int i = 0;i<360;i+=90){

        gl.model(T);
        //
        gl.draw(m_arbre);
    }

}



void Viewer::draw_axe(const Transform& T)
{
    gl.model(T);
    gl.texture(0);
    gl.lighting(false);
    gl.draw(m_axe);
    gl.lighting(true);
}

void Viewer::draw_cube(const Transform& T, unsigned int tex)
{
    gl.model(T*Scale(0.5,0.5,0.5));
    gl.texture(tex);
    gl.draw(m_cube);
}


void Viewer::manageCameraLight()
{
    // recupere les mouvements de la souris pour deplacer la camera, cf tutos/tuto6.cpp
    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
    // deplace la camera
    if((mb & SDL_BUTTON(1)) &&  (mb& SDL_BUTTON(3)))                 // le bouton du milieu est enfonce
        m_camera.translation( (float) mx / (float) window_width(), (float) my / (float) window_height());         // deplace le point de rotation
    else if(mb & SDL_BUTTON(1))                      // le bouton gauche est enfonce
        m_camera.rotation( mx, my);       // tourne autour de l'objet
    else if(mb & SDL_BUTTON(3))                 // le bouton droit est enfonce
        m_camera.move( my);               // approche / eloigne l'objet
    if (key_state(SDLK_PAGEUP) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT)) ) { m_camera.translation( 0,0.01); }
    if (key_state(SDLK_PAGEDOWN) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT)) ) { m_camera.translation( 0,-0.01); }
    if (key_state(SDLK_LEFT) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT)) ) { m_camera.translation(  0.01,0); }
    if (key_state(SDLK_RIGHT) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT)) ) { m_camera.translation( -0.01,0); }
    if (key_state(SDLK_UP) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.move( 1); }
    if (key_state(SDLK_DOWN) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.move( -1); }


    // Deplace la lumiere
    const float step = 0.1f;
    if (key_state(SDLK_RIGHT) && key_state(SDLK_LCTRL)) { gl.light( gl.light()+Vector(step,0,0)); }
    if (key_state(SDLK_LEFT) && key_state(SDLK_LCTRL)) { gl.light( gl.light()+Vector(-step,0,0)); }
    if (key_state(SDLK_UP) && key_state(SDLK_LCTRL)) { gl.light( gl.light()+Vector(0,0,-step)); }
    if (key_state(SDLK_DOWN) && key_state(SDLK_LCTRL)) { gl.light( gl.light()+Vector(0,0,step)); }
    if (key_state(SDLK_PAGEUP) && key_state(SDLK_LCTRL)) { gl.light( gl.light()+Vector(0,step,0)); }
    if (key_state(SDLK_PAGEDOWN) && key_state(SDLK_LCTRL)) { gl.light( gl.light()+Vector(0,-step,0)); }



    // (De)Active la grille / les axes
    if (key_state('h')) help();
    if (key_state('c')) { clear_key_state('c'); mb_cullface=!mb_cullface; if (mb_cullface) glEnable(GL_CULL_FACE);else glDisable(GL_CULL_FACE); }
    if (key_state('w')) { clear_key_state('w'); mb_wireframe=!mb_wireframe; if (mb_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    if (key_state('g')) { b_draw_grid = !b_draw_grid; clear_key_state('g'); }
    if (key_state('a')) { b_draw_axe = !b_draw_axe; clear_key_state('a'); }
    if (key_state('z')) { b_draw_animation=!b_draw_animation; clear_key_state('z');}

    gl.camera(m_camera);
    //draw(cube, Translation( Vector( gl.light()))*Scale(0.3, 0.3, 0.3), camera);
    //draw_param.texture(quad_texture).camera(camera).model(Translation( 3, 5, 0 )).draw(quad);

    // AXE et GRILLE
    gl.model( Identity() );
    if (b_draw_grid) gl.draw(m_grid);
    if (b_draw_axe) gl.draw(m_axe);
    if (b_draw_animation) m_anim.draw(m_camera);

     // LIGHT
    gl.texture( 0 );
    gl.lighting(false);
    gl.model( Translation( Vector( gl.light()))*Scale(0.3, 0.3, 0.3) );
    gl.draw(m_cube);
    gl.lighting(true);
}

int Viewer::quit( )
{
    return 0;
}
