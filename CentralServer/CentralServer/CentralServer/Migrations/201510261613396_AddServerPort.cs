namespace CentralServer.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class AddServerPort : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.Servers", "Port", c => c.String());
        }
        
        public override void Down()
        {
            DropColumn("dbo.Servers", "Port");
        }
    }
}
