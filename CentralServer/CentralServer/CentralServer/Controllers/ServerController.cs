using CentralServer.Models;
using CentralServer.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web;
using System.Web.Http;

namespace CentralServer.Controllers
{
    /// <summary>
    /// Class to handle all data for the Servers
    /// </summary>
    public class ServerController : ApiController
    {
        private ServerRepository serverRepository;

        /// <summary>
        /// ServerController Constructor
        /// </summary>
        public ServerController()
        {
            this.serverRepository = new ServerRepository();
        } 


        // GET api/server
        /// <summary>
        /// Use to fetch all general data on all servers, to be use by the Launcher
        /// </summary>
        /// <returns>List of servers</returns>
        public Server[] Get()
        {
            Server [] ret = serverRepository.GetAllServers();

            return ret;
        }

        // GET api/server/{id}
        /// <summary>
        /// Use to fetch all general data on one server, server is selected by his ID
        /// </summary>
        /// <param name="id">ID of the server you want to fetch</param>
        /// <returns>Return 404 not found if the server is not found / return 200 OK with the server if succeeded</returns>
        public HttpResponseMessage Get(int id)
        {
            HttpResponseMessage response;
            Server ret = serverRepository.GetServerById(id);

            if (ret == null)
                response = Request.CreateErrorResponse(HttpStatusCode.NotFound, "No server with ID : " + id);
            else
                response = Request.CreateResponse<Server>(HttpStatusCode.OK, ret);

            return response;
        }

        // POST api/server
        /// <summary>
        /// Used to add a server to the list. To be used when server is getting online/booted
        /// </summary>
        /// <param name="server">All data for the server</param>
        /// <returns>Return a HTTPReponse StatusCode Created if it succeeded or HTTPError InternalServerError if not</returns>
        public HttpResponseMessage Post(Server server)
        {
            HttpResponseMessage response;
            Server newServer;

            server.IP = String.IsNullOrEmpty(server.IP) ? HttpContext.Current.Request.UserHostAddress : server.IP;

            newServer = this.serverRepository.SaveServer(server);
            if (newServer != null)
                response = Request.CreateResponse<Server>(System.Net.HttpStatusCode.Created, newServer);
            else
                response = Request.CreateErrorResponse(HttpStatusCode.InternalServerError, serverRepository.GetLastException());

            return response;
        }

        // PUT api/server/5
        /// <summary>
        /// Used to update a server information to the list. To be used when server has general update (new player, map update etc...)
        /// The server modified is found by ID
        /// </summary>
        /// <param name="server">Set ID to select server and then fill all parameters you want to change. If params are null, they will be ignored</param>
        /// <returns>Return a HTTPResonse SattusCode InternalServerError / Return a HTTPReponse StatusCode OK with the server updated if it succeeded</returns>
        public HttpResponseMessage Put(Server server)
        {
            HttpResponseMessage response;
            Server newServer;

            newServer = this.serverRepository.UpdateServer(server.Id, server);
            if (newServer != null)
                response = Request.CreateResponse<Server>(System.Net.HttpStatusCode.OK, server);
            else
                response = Request.CreateErrorResponse(HttpStatusCode.InternalServerError, serverRepository.GetLastException());

            return response;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="server"></param>
        public void Delete(Server server)
        {
            this.serverRepository.DeleteServer(server.Id);
        }

        // DELETE api/server/5
        /// <summary>
        /// Use to delete one server by ID
        /// To be used when server is shutdown/offline
        /// </summary>
        /// <param name="id">The server ID to delete</param>
        public void Delete(int id)
        {
            this.serverRepository.DeleteServer(id);
        }
    }
}
