namespace CentralServer.Migrations
{
    using CentralServer.Models;
    using System;
    using System.Data.Entity;
    using System.Data.Entity.Migrations;
    using System.Linq;

    internal sealed class Configuration : DbMigrationsConfiguration<CentralServer.DataContexts.ServerDb>
    {
        public Configuration()
        {
            AutomaticMigrationsEnabled = true;
        }

        protected override void Seed(CentralServer.DataContexts.ServerDb context)
        {
            context.Servers.AddOrUpdate(
                new Server
                {
                    Name = "First Server",
                    CurrentMode = "Deathmatch",
                    CurrentMap = "De_dust2",
                    PlayerNumber = 3,
                    MaxPlayer = 10,
                    IP = "127.0.0.1",
                    Port = "13737",
                    DateCreated = DateTime.Now,
                    LastUpdate = DateTime.Now
                },
                new Server
                {
                    Name = "Second Server",
                    CurrentMode = "Capture The Flag",
                    CurrentMap = "De_dust2",
                    PlayerNumber = 10,
                    MaxPlayer = 20,
                    IP = "203.154.207.64",
                    Port = "13738",
                    DateCreated = DateTime.Now,
                    LastUpdate = DateTime.Now
                });
            //  This method will be called after migrating to the latest version.

            //  You can use the DbSet<T>.AddOrUpdate() helper extension method 
            //  to avoid creating duplicate seed data. E.g.
            //
            //    context.People.AddOrUpdate(
            //      p => p.FullName,
            //      new Person { FullName = "Andrew Peters" },
            //      new Person { FullName = "Brice Lambson" },
            //      new Person { FullName = "Rowan Miller" }
            //    );
            //
        }
    }
}
