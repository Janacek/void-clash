namespace CentralServer.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class Initial : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.Servers",
                c => new
                    {
                        Id = c.Int(nullable: false, identity: true),
                        Name = c.String(),
                        CurrentMode = c.String(),
                        PlayerNumber = c.Int(nullable: false),
                        MaxPlayer = c.Int(nullable: false),
                        CurrentMap = c.String(),
                        IP = c.String(),
                        DateCreated = c.DateTime(),
                        LastUpdate = c.DateTime(),
                    })
                .PrimaryKey(t => t.Id);
        }
        
        public override void Down()
        {
            DropTable("dbo.Servers");
        }
    }
}
