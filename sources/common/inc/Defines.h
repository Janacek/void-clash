/*
** Defines.h for Void Clash in /home/gegon/Void-Clash/client/src
**
** Made by brunel guillaume
** Login   <brunel_g@epitech.net>
**
** Started on  Thu Aug  1 03:06:05 2013 brunel guillaume
** Last update Wed Mar 19 11:37:02 2014 sadin_n
*/

#ifndef		DEFINES_H_
# define	DEFINES_H_

///////////////////////////////
// Revision version
// If server and client revision version do not match, client will be warned
#define	VOID_CLASH_VERSION	31

// Auto kick client timeout
#define	INACTIVITY_TIMEOUT	20.f

// Ports
#define	BIND_PORT_START		13737
#define	BIND_PORT_END		13750 // MUST BE HIGHER THAN START AND NOT TOO FAR

// DEBUG_LEVEL
// INFO >= 3
// WARNING >= 2
// ERROR >= 1
#define DEBUG_LEVEL 1
#define ENABLE_OPENGL_CHECK true

///////////////////////////////
// Shared structs

struct Point
{
	float x, y;
};

struct Vecteur
{
	float x, y;
};

struct Cercle
{
	float x, y, rayon;
};

#endif		/* !DEFINES_H_ */
