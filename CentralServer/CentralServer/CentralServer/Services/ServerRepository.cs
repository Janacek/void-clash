using CentralServer.DataContexts;
using CentralServer.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Diagnostics;
using System.Linq;
using System.Timers;
using System.Web;

namespace CentralServer.Services
{
    public class ServerRepository
    {
        #region [ === Old Code : For cache memory (no DB) === ]
//        private const string CacheKey = "ServerList";
        #endregion

        private Exception lastExceptionThrown;

//        private ServerDb db = new ServerDb();

        // Timer used to check servers uptime
        private System.Timers.Timer serverChecker;
        private System.Timers.Timer serverUpdate;

        // To modify DB data thread safely
        private Object mutexDbAccess = new Object();

        // Time of a server before he gets deleted if not updated (in seconds)
        private const double SERVER_LIFETIME = 121;

        // Time for the server checker to proc (check if server are alive)
        private const int SERVER_CHECKER_PERIOD = 75000;

        #region [ === Helpers === ]
        private Server updateServer(Server initial, Server newServer)
        {
            // ID is ignored (should be handled by the DB)
            initial.CurrentMap = String.IsNullOrEmpty(newServer.CurrentMap) ? initial.CurrentMap : newServer.CurrentMap;
            initial.CurrentMode = String.IsNullOrEmpty(newServer.CurrentMode) ? initial.CurrentMode : newServer.CurrentMode;
            initial.IP = String.IsNullOrEmpty(newServer.IP) ? initial.IP : newServer.IP;
            initial.Name = String.IsNullOrEmpty(newServer.Name) ? initial.Name : newServer.Name;
            initial.PlayerNumber = newServer.PlayerNumber != -1 ? newServer.PlayerNumber : initial.PlayerNumber;

            return initial;
        }

        public Exception GetLastException()
        {
            return lastExceptionThrown;
        }
        #endregion

        public ServerRepository()
        {
            mutexDbAccess = new Object();
            serverChecker = new System.Timers.Timer(SERVER_CHECKER_PERIOD);
//            serverUpdate = new System.Timers.Timer(45000);
            serverChecker.Elapsed += serverChecker_Elapsed;
//            serverUpdate.Elapsed += serverUpdate_Elapsed;
            serverChecker.Start();
//            serverUpdate.Start();

            #region [ === Old Code : For cache memory (no DB) === ]
            //var ctx = HttpContext.Current;

            //if (ctx != null)
            //{
            //    if (ctx.Cache[CacheKey] == null)
            //    {
            //        var servers = new Server[]
            //        {
            //            // Temporary data for test purposes
            //            new Server
            //            {
            //                Id = 1, Name = "First Server", CurrentMode = Server.ServerMode.Deathmatch.ToString(), PlayerNumber = 3, CurrentMap = "De_dust2", IP = "127.0.0.1"
            //            },
            //            new Server
            //            {
            //                Id = 2, Name = "Second Server", CurrentMode = Server.ServerMode.CaptureTheFlag.ToString(), PlayerNumber = 10, CurrentMap = "De_dust2", IP = "203.154.207.64"
            //            },
            //        };

            //        ctx.Cache[CacheKey] = servers;
            //    }
            //}
            #endregion
        }
        void serverUpdate_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    DateTime now = DateTime.Now;
                    foreach (Server s in db.Servers)
                    {
                        s.LastUpdate = now;
                    }
                }
            }
        }

        void serverChecker_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    DateTime now = DateTime.Now;
                    List<Server> removeList = new List<Server>();
                    TimeSpan? diff;
                    foreach (Server s in db.Servers)
                    {
                        if (s.LastUpdate == null)
                            diff = null;
                        else
                            diff = now - s.LastUpdate.Value;
                        Trace.Write("Checing timers", "Now is : " + now.ToString() + " && LastUpdate is : " + s.LastUpdate + " && Diff is : " + diff);
                        if (diff == null || diff.Value.TotalSeconds > SERVER_LIFETIME)
                        {
                            Trace.Write("Remove server cauz of timer", "Now is : " + now.ToString() + " && LastUpdate is : " + s.LastUpdate + " && Diff is : " + diff);
                            removeList.Add(s);
                        }
                    }
                    if (removeList.Count > 0)
                    {
                        foreach (Server s in removeList)
                        {
                            db.Servers.Remove(s);
                        }
                        //db.Servers.RemoveRange(removeList);
                        db.SaveChanges();
                    }
                }
            }
        }

        public Server[] GetAllServers()
        {
            Server[] ret;
            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    ret = db.Servers.ToArray();
                }
            }
            return (ret);

            #region [ === Old Code : For cache memory (no DB) === ]
            //var ctx = HttpContext.Current;

            //if (ctx != null)
            //{
            //    return (Server[])ctx.Cache[CacheKey];
            //}

            //return new Server[]
            //{
            //    new Server
            //    {
            //        Id = 0,
            //        Name = "Placeholder"
            //    }
            //};
            #endregion
        }

        public Server GetServerById(int id)
        {
            Server ret;

            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    ret = db.Servers.Find(id);
                }
            }
            return ret;

            #region [ === Old Code : For cache memory (no DB) === ]
            //var ctx = HttpContext.Current;

            //if (ctx != null)
            //{
            //    var currentData = (Server[])ctx.Cache[CacheKey];

            //    var server = currentData.FirstOrDefault((s) => s.Id == id);
            //    if (server != default(Server))
            //        return server;
            //}

            //return null;
            #endregion
        }

        /// <summary>
        /// Use to create a new server
        /// </summary>
        /// <param name="server">The server you want to create</param>
        /// <returns>Return null if fails and LastExceptionThrown is updated / Return the created server if succeeded</returns>
        public Server SaveServer(Server server)
        {
            Server newServer;

            server.DateCreated = DateTime.Now;
            server.LastUpdate = DateTime.Now;
            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    newServer = db.Servers.Add(server);
                    db.SaveChanges();
                }
            }

            return newServer;

            #region [ === Old Code : For cache memory (no DB) === ]
            //var ctx = HttpContext.Current;

            //if (ctx != null)
            //{
            //    try
            //    {
            //        var currentData = ((Server[])ctx.Cache[CacheKey]).ToList();
            //        server.Id = currentData.Select(s => s.Id).Max();
            //        currentData.Add(server);
            //        ctx.Cache[CacheKey] = currentData.ToArray();

            //        return server;
            //    }
            //    catch (Exception ex)
            //    {
            //        this.lastExceptionThrown = ex;
            //        Console.WriteLine(ex.ToString());
            //        return null;
            //    }
            //}
            //return null;
            #endregion
        }

        /// <summary>
        /// Use to update a server
        /// </summary>
        /// <param name="id">The ID of the server you want to update</param>
        /// <param name="server">The data you want to update (ID is ignored)</param>
        /// <returns>Return the updated server if it succeeded / return null if it failed and lastExceptionThrown is updated</returns>
        public Server UpdateServer(int id, Server server)
        {
            Server oldServer;

            server.Id = id;
            server.LastUpdate = DateTime.Now;
            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    oldServer = db.Servers.Find(id);
                    oldServer.Copy(server);

                    db.Entry(oldServer).State = EntityState.Modified;
                    db.SaveChanges();
                }
            }
            return server;

            #region [ === Old Code : For cache memory (no DB) === ]
            //var ctx = HttpContext.Current;
            //Server newServer = null;

            //if (ctx != null)
            //{
            //    try
            //    {
            //        var currentData = ((Server[])ctx.Cache[CacheKey]).ToList();
            //        // Cannot have a double of IDs because it is handled by the DB (Id is the key)
            //        var serverToUpdate = currentData.FirstOrDefault((s) => s.Id == id);
            //        if (serverToUpdate == default(Server))
            //            throw new Exception("No server found with this ID : " + id);
            //        else
            //            newServer = updateServer(serverToUpdate, server);
            //        ctx.Cache[CacheKey] = currentData.ToArray();

            //        return newServer;
            //    }
            //    catch (Exception ex)
            //    {
            //        this.lastExceptionThrown = ex;
            //        Console.WriteLine(ex.ToString());
            //        return null;
            //    }
            //}
            //return null;
            #endregion
        }

        public bool DeleteServer(int id)
        {
            Server server;
            using (var db = new ServerDb())
            {
                lock (mutexDbAccess)
                {
                    server = db.Servers.Find(id);
                    server = db.Servers.Remove(server);
                    db.SaveChanges();
                }
            }
            if (server != null)
                return true;
            return false;

            #region [ === Old Code : For cache memory (no DB) === ]
            //var ctx = HttpContext.Current;

            //if (ctx != null)
            //{
            //    try
            //    {
            //        var currentData = ((Server[])ctx.Cache[CacheKey]).ToList();
            //        var serverToDelete = currentData.FirstOrDefault((s) => s.Id == id);
            //        if (serverToDelete == default(Server))
            //            return false;
            //        currentData.Remove(serverToDelete);
            //        ctx.Cache[CacheKey] = currentData.ToArray();
            //        return true;
            //    }
            //    catch (Exception ex)
            //    {
            //        this.lastExceptionThrown = ex;
            //        Console.WriteLine(ex.ToString());
            //        return false;
            //    }
            //}
            //return false;
            #endregion
        }
    }
}