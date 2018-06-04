#if defined(linux) || defined(__linux)

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cstddef>
#include <dirent.h>
#include "Files.hpp"

void	*Files::first(const char *dirname, char **filename)
{
  void	*dir = NULL;
  void	*handle = NULL;

  if (!(dir = (void *) opendir(dirname)) || !(handle = readdir((DIR *)dir)))
    return (NULL);
  *filename = new char[256];
  memset(*filename, 0, 256);
  memcpy(*filename, ((struct dirent *)handle)->d_name, 256);
  return (dir);
}

char *Files::next(void *dir)
{
  char	*filename = new char[256];
  void		*handle = NULL;

  memset(filename, 0, 256);
  if (!(handle = readdir((DIR *)dir)))
    return (NULL);
  memcpy(filename, ((struct dirent *)handle)->d_name, 256);
  return (filename);
}

bool	Files::close(void *dir)
{
  bool	closed = false;

  closed = (closedir((DIR *)dir) == 0);
  return (closed);
}

bool    Files::isFile(const char *filename)
{
  struct stat buff;

  stat(filename, &buff);
  return (S_ISREG(buff.st_mode));
}

char	*Files::getPath(const char *folder, const char *file)
{
  char	*path = new char[strlen(folder) + strlen(file) + 2];

  memset(path, 0, strlen(folder) + strlen(file) + 2);
  memcpy(path, folder, strlen(folder));
  memcpy(path + strlen(folder), "/", 1);
  memcpy(path + strlen(folder) + 1, file, strlen(file));
  return (path);
}

#endif
