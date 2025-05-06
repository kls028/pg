<?php
session_start();

require '../vendor/autoload.php';
require '../config/DB.php';
$db = get_db(); 

define('BASE_PATH', '/sudoku_gallery/');

// Simple routing logic with defaults
$controller = isset($_GET['controller']) ? $_GET['controller'] : 'gallery';
$action = isset($_GET['action']) ? $_GET['action'] : 'galleryAction';

require '../models/User.php';
$userModel = new User($db);

require '../models/Image.php';
$imageModel = new Image($db);

require '../controllers/ErrorController.php';
$errorController = new ErrorController();
// Load the necessary controller
switch ($controller) {
    case 'user':
        // Load the user controller
        require '../controllers/UserController.php';
        $userController = new UserController($userModel);
        break;

    case 'gallery':
        require '../controllers/GalleryController.php';
        $galleryController = new GalleryController($imageModel, $userModel);
        break;

    default:
        $errorController->_404();
        break;
}

// Check if the controller exists and the action method is valid
if (isset($userController) && method_exists($userController, $action)) {
    $userController->$action();
} elseif (isset($galleryController) && method_exists($galleryController, $action)) {
    $galleryController->$action();
} else {
    $errorController->_404();
    exit();
}

