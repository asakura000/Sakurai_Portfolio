using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Firebase.Firestore;
using System.Threading.Tasks;
using UnityEngine.UI;
using System.Linq;
using System.Threading;
using Firebase.Auth;

public class StatsManager : MonoBehaviour
{
    protected bool operationInProgress;
    protected Task previousTask;
    protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
    private System.DateTime dt;

    // name of the collection container that documents get stored into 
    protected string collectionPathTop = "Hit Counts";
    protected string collectionPathSub = "Trials";
    protected string documentTopLevel = "";
    protected string documentSubLevel = "";

    public HitStats hitStats;
    public Text text;
    
    //Boilerplate taskmanagement code made by the Firebase People
    ////////////////////////////////////////////////////////////////////////////
    class WaitForTaskCompletion : CustomYieldInstruction
    {
        Task task;
        StatsManager dbManager;
        protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

        // Create an enumerator that waits for the specified task to complete.
        public WaitForTaskCompletion(StatsManager dbManager, Task task)
        {
            dbManager.previousTask = task;
            dbManager.operationInProgress = true;
            this.dbManager = dbManager;
            this.task = task;
        }

        // Wait for the task to complete.
        public override bool keepWaiting
        {
            get
            {
                if (task.IsCompleted)
                {
                    dbManager.operationInProgress = false;
                    dbManager.cancellationTokenSource = new CancellationTokenSource();
                    if (task.IsFaulted)
                    {
                        string s = task.Exception.ToString();
                        Debug.Log(s);
                    }
                    return false;
                }
                return true;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    //Get the instance of the Firestore database 
    protected FirebaseFirestore db
    {
        get
        {
            return FirebaseFirestore.DefaultInstance;
        }
    }


    private CollectionReference GetCollectionReference()
    {
        return db.Collection(collectionPathTop);

    }

//Gets the reference to the docuement in Firstore
//Here I am just setting the document manually to the userID of the sign in user
 
    private DocumentReference GetDocumentReference()
    {

        //documentTopLevel = FirebaseAuth.DefaultInstance.CurrentUser.UserId;

        documentTopLevel = PlayerPrefs.GetInt("userIndex").ToString();

        // assign a unique document ID to the Trials subcollection

        documentSubLevel = "Trial " + dt.ToString("yyyy-MM-dd-H-mm-ss");

        return GetCollectionReference().Document(documentTopLevel).Collection(collectionPathSub).Document(documentSubLevel);
    }

    // Need function here that detects whether collision happened and then updates count to firebase

    public void LogGameStats()
    {

        Debug.Log("You called me again?");
        var data = new Dictionary<string, object>{
          {"Number of Targets Collected", hitStats.targetCollectedCount},
          {"Number of Walls Hit", hitStats.wallHitCount},
          {"Number of Hazards Hit", hitStats.hazardHitCount},

          };

          StartCoroutine(WriteDoc(GetDocumentReference(), data));
          Debug.Log("I logged something");
           
       
    }

    
    public void OnCollisionEnter(Collision collision)
    {
        Debug.Log("You called me?");

        if(collision.gameObject.tag == "Wall")
        {
            LogGameStats();
        }
        else if(collision.gameObject.tag == "Hazard")
        {
            LogGameStats();
        }
    }

    public void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.tag == "Target")
        {
            LogGameStats();
        }
    }

    // ***  what does this do?

    public void readDataButton()
    {
        StartCoroutine(ReadDoc(GetDocumentReference()));
    }

    private static string DictToString(IDictionary<string, object> d)
    {
        return "{ " + d
            .Select(kv => "(" + kv.Key + ", " + kv.Value + ")")
            .Aggregate("", (current, next) => current + next + ", ")
            + "}";
    }

    //Writes the data 
    private IEnumerator WriteDoc(DocumentReference doc, IDictionary<string, object> data)
    {
        Task setTask = doc.SetAsync(data);
        yield return new WaitForTaskCompletion(this, setTask);
        if (!(setTask.IsFaulted || setTask.IsCanceled))
        {
            Debug.Log("Data written");
        }
        else
        {
            Debug.Log("Error");
        }
    }


    private IEnumerator ReadDoc(DocumentReference doc)
    {
        Task<DocumentSnapshot> getTask = doc.GetSnapshotAsync();
        yield return new WaitForTaskCompletion(this, getTask);
        if (!(getTask.IsFaulted || getTask.IsCanceled))
        {
            DocumentSnapshot snap = getTask.Result;
            IDictionary<string, object> resultData = snap.ToDictionary();

            text.text = DictToString(resultData);
            Debug.Log("Data read");
        }
        else
        {
            text.text = "Error";
        }
    }

    void Start()
    {
        dt = System.DateTime.Now;
    }

}
