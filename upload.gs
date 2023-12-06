function getFolderPath(folder) {
  var path = "";
  
  while (folder.getParents().hasNext()) {
    folder = folder.getParents().next();
    path = "/" + folder.getName() + path;
  }
  
  return path+'/';
}

// Idea is we don't create folders at root level, only levels below
function doPost(e) {
  var data = Utilities.base64Decode(e.parameters.data);
  var nombreArchivo = Utilities.formatDate(new Date(), "GMT-3", "yyyyMMdd_HHmmss")+".jpg";
  var blob = Utilities.newBlob(data, e.parameters.mimetype, nombreArchivo );

  // Save the photo to Google Drive
  var ESP32_CAM = "ESP32-CAM"; // the path for this folder is /Photos/ESP32-CAM
  var folder, folders = DriveApp.getFoldersByName(ESP32_CAM); // this gets all the folder and sub-folders.

  // check if ESP32_CAM exists. If it does print it out, else check if /Photos exists and attempt to create ESP32_CAM inside it
  if (folders.hasNext()) { // this method checks if there are more folders to process ? true : false
    folder = folders.next();
    Logger.log(folder.getName() + " exists in: " + getFolderPath(folder) + '!');
  } else {
    var Photos = "Photos";
    var photo_folder = DriveApp.getFoldersByName(Photos);
    var p_folder;

    // if the Photos folder exists, create a ESP32_CAM folder inside it, else print Photos folder not found
    if (photo_folder.hasNext()) {
      p_folder = photo_folder.next();
      var childFolder = p_folder.createFolder(ESP32_CAM);
      Logger.log("Folder: " + childFolder.getName() + " created at: " + getFolderPath(childFolder));
    }else {
      p_folder = null;
      Logger.log("Folder: " + Photos + " not found. Cannot create a sub-folder inside non-existing folers. Please create this folder first.");
    }
  }

  var file = folder.createFile(blob); 
  return ContentService.createTextOutput('Completo')
}