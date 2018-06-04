using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CentralServer.Models
{
    public class Server
    {
        public enum ServerMode
        {
            Deathmatch,
            TeamDeathmatch,
            CaptureTheFlag,
            UNKNOWN
        }

        public Server()
        {
            PlayerNumber = -1;
            Id = -1;
            MaxPlayer = -1;
        }

        public int Id { get; set; }

        public string Name { get; set; }
        public string CurrentMode { get; set; }
        public int PlayerNumber { get; set; }
        public int MaxPlayer { get; set; }
        public string CurrentMap { get; set; }
        public string IP { get; set; }
        public string Port { get; set; }
        public DateTime? DateCreated { get; set; }
        public DateTime? LastUpdate { get; set; }

        public void Copy(Server s)
        {
            Id = s.Id;
            Name = s.Name;
            CurrentMode = s.CurrentMode;
            PlayerNumber = s.PlayerNumber;
            MaxPlayer = s.MaxPlayer;
            CurrentMap = s.CurrentMap;
            if (s.IP != null)
                IP = s.IP;
            if (s.DateCreated != null)
                DateCreated = s.DateCreated;
            LastUpdate = s.LastUpdate;
        }

    }
}