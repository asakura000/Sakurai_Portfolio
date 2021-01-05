using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Firebase.Firestore;
using System.Threading.Tasks;
using UnityEngine.UI;
using System.Linq;
using System.Threading;
using Firebase.Auth;

public class TrajectoryToFirebase : MonoBehaviour
{
    protected bool operationInProgress;
    protected Task previousTask;
    protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

    // name of the collection container that documents get stored into 
    protected string collectionPathTop = "Trajectory Log";
    protected string collectionPathSub = "Entries";
    protected string documentTopLevel = "";
    protected string documentSubLevel = "";

    public float samplingRate = 1f;
    private System.DateTime dt;
    private Vector3 lastpos = Vector3.zero;
    private Quaternion lastrot = Quaternion.identity;
    private List<Dictionary<string, object>> tracker = new List<Dictionary<string, object>>();

    //public GameStats gameStats;
    public Text text;

    //Boilerplate taskmanagement code made by the Firebase People
    ////////////////////////////////////////////////////////////////////////////
    class WaitForTaskCompletion : CustomYieldInstruction
    {
        Task task;
        TrajectoryToFirebase dbManager;
        protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

        // Create an enumerator that waits for the specified task to complete.
        public WaitForTaskCompletion(TrajectoryToFirebase dbManager, Task task)
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

        //documentId = FirebaseAuth.DefaultInstance.CurrentUser.UserId;

        documentTopLevel = "User 1";
        
      
        return GetCollectionReference().Document(documentTopLevel);
    }

    // Need function here that sends log data to firestore
    // make this so that it checks to see if the document is null, and if it is not, it appends to the previous entry
    // Firebase transaction?


    public void TrackTrajectory()
    {
        Debug.Log("Trajectory Tracker Called");

        if (transform.position != lastpos || transform.rotation != lastrot)
        {


            var data = new Dictionary<string, object>{
                {"Time", System.Math.Round(Time.time, 2) },
                {"Pos X",  System.Math.Round(transform.position.x, 3)},
                {"Pos Y",  System.Math.Round(transform.position.y, 3)},
                {"Pos Z",  System.Math.Round(transform.position.z, 3)},
                {"Rot X",  System.Math.Round(transform.rotation.x, 3)},
                {"Rot Y",  System.Math.Round(transform.rotation.y, 3)},
                {"Rot Z",  System.Math.Round(transform.rotation.z, 3)}, 

             };

            tracker.Add(data);
           
            Debug.Log("I recorded trajectory data");
            Debug.Log(tracker.Count);

            lastpos = transform.position;
            lastrot = transform.rotation;
         
        }

        documentTopLevel = "User 1";

        int counter = 0;

        foreach (var data in tracker)
        {
            counter++;
            documentSubLevel = counter.ToString();
            DocumentReference docref = GetCollectionReference().Document(documentTopLevel).Collection(collectionPathSub + dt.ToString(" yyyy-MM-dd-H-mm-ss")).Document(documentSubLevel);
            StartCoroutine(WriteDoc(docref, data));
            Debug.Log("I am writing new data");
        }

    }

   
    void OnApplicationQuit()
    {
        Debug.Log("Ended game");
    }


    // starts "timer" to start recording position and orientation?
    public void Start()
    {
        dt = System.DateTime.Now;
        Debug.Log(dt.ToString("yyyy-MM-dd-H-mm-ss"));
        InvokeRepeating("TrackTrajectory", 0f, 1f / samplingRate);
        
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

    // Writes the data 
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

    // Reads data
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



}
