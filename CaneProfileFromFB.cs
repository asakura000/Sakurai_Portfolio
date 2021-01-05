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

// Trying to retrieve data manually entered into Firestore into Unity game scene 

public class CaneProfileFromFB : MonoBehaviour
{
    protected bool operationInProgress;
    protected Task previousTask;
    protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

    public float length;
    public string material;

    private List<string> userRoster = new List<string>();
    
    // name of the collection container that documents get stored into 
    protected string collectionPath = "Cane Settings";
    protected string documentId = "";

     
    //Boilerplate taskmanagement code made by the Firebase People
    ////////////////////////////////////////////////////////////////////////////
    class WaitForTaskCompletion : CustomYieldInstruction
    {
        Task task;
        CaneProfileFromFB dbReader;
        protected CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

        // Create an enumerator that waits for the specified task to complete.
        public WaitForTaskCompletion(CaneProfileFromFB dbReader, Task task)
        {
            dbReader.previousTask = task;
            dbReader.operationInProgress = true;
            this.dbReader = dbReader;
            this.task = task;
        }

        // Wait for the task to complete.
        public override bool keepWaiting
        {
            get
            {
                if (task.IsCompleted)
                {
                    dbReader.operationInProgress = false;
                    dbReader.cancellationTokenSource = new CancellationTokenSource();
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
    ///

    async void GetAllDocs()
    {
        // query all documents in the collection called users
        // convert just the user ID into a list of strings 

        Query allUsersQuery = db.Collection("Users");
        QuerySnapshot allUsersQuerySnapshot = await allUsersQuery.GetSnapshotAsync();

        // Creates a list -> [userID-1, userID-2, userID-3, .....] in order from top to bottom on the Firestore screen

        foreach (DocumentSnapshot documentSnapshot in allUsersQuerySnapshot.Documents)
        {
            userRoster.Add(documentSnapshot.Id);
            Debug.Log(userRoster.Count);
            Debug.Log(documentSnapshot.Id);

        }

    }

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
        //documentId = FirebaseAuth.DefaultInstance.CurrentUser.UserId;

        // need a way to identify current user and assign same document ID

        return GetCollectionReference().Document(documentId);
        
    }


    // Reads the Data from Firestore Database and converts to variables cane length and cane tip material

    private IEnumerator ReadDoc(DocumentReference doc)
    {
        Task<DocumentSnapshot> getTask = doc.GetSnapshotAsync();

        yield return new WaitForTaskCompletion(this, getTask);

        if (!(getTask.IsFaulted || getTask.IsCanceled))
        {
            // Gets a snapshot of the Firestore database at the time this is called (start)

            DocumentSnapshot snap = getTask.Result;

            // converts snapshot to dictionary of <string, object>

            IDictionary<string, object> resultData = snap.ToDictionary();

            // converts dictionary<string,object> to dictionary<string,string>

            Dictionary<string, string> dString = resultData.ToDictionary(k => k.Key, k => k.Value.ToString());

            // converts dictionary <string, string> into a string array of just the values then
            
            string[] values = new string[dString.Count];
            dString.Values.CopyTo(values, 0);

            // finally, ssigns public float cane length to first item in array and public string cane tip material to second item

            length = float.Parse(values[0]);
            material = values[1];

            // add this same line to 3D sound script

            PlayerPrefs.SetString("CaneTip", material);

            Debug.Log("Data read from Firebase"); 
            
        }
        else
        {
            Debug.Log("Error");
        }
    }

   
    public void Start()
    {
        StartCoroutine(ReadDoc(GetDocumentReference()));
        GetAllDocs();
    }
}
