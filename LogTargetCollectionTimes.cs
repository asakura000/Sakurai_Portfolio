using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Firebase.Firestore;
using System.Threading.Tasks;
using UnityEngine.UI;
using System.Linq;
using System.Threading;
using Firebase.Auth;
using System;

public class LogTargetCollectionTimes : MonoBehaviour
{
    public Text text;
    protected bool operationInProgress;
    protected Task previousTask;
    protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
    // name of the collection container that documents get stored into 
    protected string collectionPath = "Target Collection Times";
    protected string documentId = "";

    private float tt1, tt2, tt3, tt4, tt5;
    private float timer;
    private float showTime;
    private float tempTime;

    //Boilerplate taskmanagement code made by the Firebase People
    ////////////////////////////////////////////////////////////////////////////
    class WaitForTaskCompletion : CustomYieldInstruction
    {
        Task task;
        LogTargetCollectionTimes dbManager;
        protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

        // Create an enumerator that waits for the specified task to complete.
        public WaitForTaskCompletion(LogTargetCollectionTimes dbManager, Task task)
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
        return db.Collection(collectionPath);
    }

    //Gets the reference to the docuement in Firstore
    //Here I am just setting the document manually to the userID of the sign in user
    private DocumentReference GetDocumentReference()
    {




        if (documentId == "")
        {
            //return GetCollectionReference().Document();
            documentId = "User 1";
        }
        return GetCollectionReference().Document(documentId);
    }

    public void TriggerEnter(Collider other)
    {
        if(other.gameObject.name == "Target")
        {
            tt1 = 1f;
            LogTargetTimes();
        }
        else if(other.gameObject.name == "Target(1)")
        {

            tt2 = 2f;
            LogTargetTimes();

        }
        else if (other.gameObject.name == "Target(2)")
        {

            tt3 = 3f;
            LogTargetTimes();

        }
        else if (other.gameObject.name == "Target(3)")
        {

            tt4 = 4f;
            LogTargetTimes();
        }
        else if (other.gameObject.name == "Target(4)")
        {
            tt4 = 5f; 
            LogTargetTimes();
        }
      
    }

    public void LogTargetTimes()
    {

        Debug.Log("Sending target times to Firestore");
        var data = new Dictionary<string, object>{

            // Enter times from the stopwatch for each object from the time array in the TimeController script

          {"Target 1", tt1},
          {"Target 2", tt2},
          {"Target 3", tt3},
          {"Target 4", tt4},
          {"Target 5", tt5},

         };

        StartCoroutine(WriteDoc(GetDocumentReference(), data));
        Debug.Log("I logged target times.");


    }

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

    void Update()
    {
        timer += Time.deltaTime;
        //Debug.Log(timer);
    }
}
