//
// Player.hpp for Void Clash in /home/gegon/Void-Clash/client/src
// 
// Made by brunel guillaume
// Login   <brunel_g@epitech.net>
// 
// Started on  Sat Aug  3 21:22:34 2013 brunel guillaume
// Last update Tue Feb  4 04:47:03 2014 Gegon
//

#ifndef		PLAYER_HPP_
# define	PLAYER_HPP_

#include	<utility>
#include	<SFML/System.hpp>
#include	"AObject.hpp"
#include	"Actions.hpp"
#include	"WeaponManager.hpp"

class	GameEngine;
class	Bomb;
class	Flag;
class	AI;

#define	GLOBAL_CAP_SPEED	15.0f

class	Player : public AObject
{
public:
  Player(float, float, float, float, bool);
  virtual ~Player(void);

  virtual bool	update(void);

  void	setAI();
  AI *getAI();

  // Input
  void	gatherInput();
  void	calculateNormalizedAim();

  // Movement
  void	checkMove(void);
  void	movePlayer(float projX, float projY);
  bool	maxSpeedReached(float maxSpeed, float projX, float projY);
  void	applyFriction(float, float);

  // Velocity weapon
  void	addPlayerVelocity(float *dirX, float *dirY);
  void	checkVelocity();

  // Bomb
  const std::shared_ptr<Bomb>	&getBombOnHold(void) const;
  void	deleteBombOnHold(void);
  void	setBombOnHold(const std::shared_ptr<Bomb>&);

  // Flag
  void		setFlag(const std::shared_ptr<Flag>	 &flag);
  const std::shared_ptr<Flag>	 &getFlag() const;

  // Getterz
  bool		isControled(void) const;
  const int&	getAccel(void) const;
  const int&	getRadius(void)	const;
  bool		isInsideRespawn();
  bool		isInsideGravityField();
  sf::Int16	getLife(void) const;
  sf::Int16	getScore(void) const;
  sf::Int16	getKills(void) const;
  sf::Int16	getKillsStreak(void) const;
  sf::Int16	getDeaths(void) const;
  float		getLatency() const;
  float		getPlayerVelocity();
  const std::pair<float, float>	&getAim(void) const;
  const std::pair<float, float>	&getAimNormalized() const;
  const float		&getEnergy(void) const;
  const std::string		&getNick() const;
  const	s_actions		&getActions() const;
  t_weapon	*getWeapons(bool primary, bool alt);
  bool		isWeaponActive(t_weapon *weaponToCheck);
  t_weapon	*getShield() const;
  WeaponManager* getWeaponsManager(void);
  bool	isActive();

  // Setterz
  void		setInsideRespawn();
  void		setInsideGravityField();
  void		setNick(const std::string &nick);
  void		plusKills(int inc = 1);
  void		plusKillsStreak(int inc = 1);
  void		plusDeaths(void);
  void		resetKillsStreak();
  void		reset();
  void		setScore(sf::Int16 score);
  void		setEnergy(float energy);
  void		setLatency(float latency);
  void		setWeapons(t_weapon *primary, t_weapon *primaryAlt, t_weapon *secondary, t_weapon *secondaryAlt);
  void		setShield(t_weapon *shield);
  void		setAim(float aimX, float aimY);
  void		setActive(bool status);

  // Life
  bool	isInvulnerable() const;
  void	isHitBy(const std::shared_ptr<AObject> &hitter, float dmg);
  void	isHealedFor(sf::Int16 heal);
  bool	checkDeath();
  void	startRespawnSequence();
  float	getTimeBeforeRespawn();
  void	respawn(void);
  bool	respawnPosition(void);
  bool	isRespawning();
  void	setDmgDone(sf::Uint32 dmg);
  sf::Uint32	getDmgDone();

  // Join time
  const sf::Time	&getMapTime() const;
  void			setMapTime(sf::Time time);

  friend sf::Packet& operator >>(sf::Packet& packet, Player &m);

private:
	bool init = false;
  sf::Clock	_clock;
  float		_currentTime;
  float		_killedTime;
  sf::Time	_respawnTime;
  std::string	_nick;
  float		_latency;
  bool		_controled;
  bool		_insideGravityField;
  bool		_insideRespawn;
  float		_playerVelocity;
  t_weapon	*_shield;
  bool		_active;

  float		_energy;
  sf::Int16	_deaths;
  sf::Int16	_kills;
  sf::Int16	_killsStreak;
  sf::Int16	_life;
  sf::Int16	_score;
  sf::Uint32	_dmg;
  bool		_invul;
  bool		_isActive;

  std::shared_ptr<Flag>	_flag;

  sf::Time	_mapTime;

  std::pair<float, float>	_aim;
  std::pair<float, float>	_normalizedAim;

  AI		*_ai;

  s_actions _actions;

  WeaponManager	*_weaponManager;
  friend class WeaponManager;
  friend class AI;
};

sf::Packet& operator >>(sf::Packet& packet, Player &m);

#endif
