using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForTest
{
    class Program
    {
        static private System.Timers.Timer serverChecker;
        static private System.Timers.Timer serverUpdate;
        static private List<DateTime?> list;
        static private DateTime? test;

        static void Main(string[] args)
        {
            list = new List<DateTime?>();
            list.Add(DateTime.Now);
            list.Add(DateTime.Now);
            test = DateTime.Now;

            serverChecker = new System.Timers.Timer(30000);
            serverChecker.Elapsed += serverChecker_Elapsed;
            serverChecker.Start();
            serverUpdate = new System.Timers.Timer(45000);
            serverUpdate.Elapsed += serverUpdate_Elapsed;
            serverUpdate.Start();

            Console.ReadKey();
            serverChecker.Stop();
        }

        static void serverChecker_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            DateTime now = DateTime.Now;
            TimeSpan? diff;

            List<DateTime?> removeList = new List<DateTime?>();
            foreach (DateTime? s in list)
            {
                if (s == null)
                    diff = null;
                else
                    diff = now - s.Value;
                if (diff == null || diff.Value.TotalSeconds > 61)
                    removeList.Add(s);
            }

            if (test == null)
                diff = null;
            else
                diff = now - test.Value;
            if (diff == null || diff.Value.TotalSeconds > 61)
                Console.WriteLine("Removing");
            Console.WriteLine("Test is : " + test.ToString());
            Console.WriteLine("Now is : " + now.ToString());
            Console.WriteLine("Diff is : " + diff.ToString() + " && Diff.TotalSeconds is : " + diff.Value.TotalSeconds);
        }

        static void serverUpdate_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            test = DateTime.Now;
        }

    }
}
