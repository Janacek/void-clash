using CentralServer.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Web;

namespace CentralServer.DataContexts
{
    public class ServerDb : DbContext
    {
        public ServerDb() : base("DefaultConnection")
        {

        }
        public DbSet<Server> Servers { get; set; }
    }
}   