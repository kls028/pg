<?php
class UserController {
    private $userModel;

    public function __construct($userModel) {
        $this->userModel = $userModel;
    }

    public function registerAction() {
        $error = null;
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            // Check if the username is already taken
            $existingUsername = $this->userModel->findByUsername($_POST['username']);
            $existingEmail = $this->userModel->findByEmail($_POST['email']);
            if ($existingUsername) {
                $error = "Username is already taken. Please choose a different one.";
            } elseif ($existingEmail) {
                $error = "Email is already taken. Please choose a different one.";
            } elseif ($_POST['password'] !== $_POST['password_confirm']){
                $error = "Passwords do not match. Please try again.";
            } else {
                // Register the user if username is not taken
                $this->userModel->register($_POST['username'], $_POST['email'], $_POST['password']);
                header('Location: app.php?controller=user&action=loginAction');
                exit();
            }
        }
        include '../views/register.php';
    }
    

    public function loginAction() {
        $error = null;
        $success = null;
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $user = $this->userModel->login($_POST['username'], $_POST['password']);
            if ($user) {
                $_SESSION['user_id'] = $user->_id;
                $success = "Successfully logged in.";
                // Redirect to gallery page
                header('Location: app.php?controller=gallery&action=galleryAction');
                exit();
            } else {
                $error= "Incorrect login or password. Try again.";
            }
        }
        include '../views/login.php';
    }

    public function logoutAction() {
        // Destroy session on logout and redirect back to login page
        session_destroy();
        header('Location: app.php?controller=user&action=loginAction');
        exit();
    }
}
?>
