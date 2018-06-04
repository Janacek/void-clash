#ifndef FILES_HPP_
# define FILES_HPP_

class Files
{
public:
  // Manipulation
  static void		*first(const char *, char **);
  static char		*next(void *);
  static bool		close(void *);

  // Utility
  static bool		isFile(const char *);
  static char		*getPath(const char *, const char *);
};

#endif /* FILES_HPP_ */
