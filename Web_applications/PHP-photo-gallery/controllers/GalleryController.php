<?php
require_once '../models/Image.php';
require_once '../models/User.php';
class GalleryController {
    private $imageModel;
    private $userModel;
    private $isSelected;
    public function __construct($imageModel, $userModel) {
        $this->imageModel = $imageModel;
        $this->userModel = $userModel;
    }

    public function uploadAction() {
        if ($this->userModel->isLoggedIn()) {
            $user = $this->userModel->getUsername($_SESSION['user_id']);
            $userId = $_SESSION['user_id'];
        } else {
            $user = null;
            $userId = null;
        }
    
        $errors = [];
    
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['image'])) {
            $file = $_FILES['image'];
    
            $allowedTypes = ['image/png', 'image/jpg'];
            if (!in_array($file['type'], $allowedTypes)) {
                $errors[] = "Invalid file type. Only PNG and JPG are allowed.";
            }

            if ($file['size'] > 1048576) {
                $errors[] = "File size exceeds the maximum limit of 1MB.";
            }
    
            if (empty($errors)) {
                $this->imageModel->uploadImage(
                    $file, 
                    $_POST['author'], 
                    $_POST['title'], 
                    $_POST['watermark'], 
                    $userId,
                    $_POST['status'] === 'public' 
                );
                header('Location: app.php?controller=gallery&action=galleryAction');
                exit();
            }
        }
        include '../views/upload.php';
    }
    
    
    public function galleryAction() {
        $page = isset($_GET['page']) ? (int)$_GET['page'] : 1; 
        $limit = 8;
        $searchQuery = isset($_POST['searchQuery']) ? $_POST['searchQuery'] : '';
       
        if ($searchQuery) {
            // Przeszukiwanie po tytule lub autorze
            $publicImages = $this->imageModel->searchImages($searchQuery);
            $totalPublicImages = $this->imageModel->getSearchImagesCount($searchQuery);
            $publicImageCount = count($totalPublicImages);
        } else {
            $publicImages = $this->imageModel->getPublicImages($page, $limit);        
            $totalPublicImages = $this->imageModel->getTotalPublicImagesCount();
            $publicImageCount = count($totalPublicImages);
        }
        if ($this->userModel->isLoggedIn()) {
            $user = $this->userModel->getUsername($_SESSION['user_id']);
            $privateImages = $this->imageModel->getPrivateImages($_SESSION['user_id']);
            $privateImageCount = count($privateImages);
        }  else {
            $user = null;
            $privateImages = [];
            $privateImageCount = 0;
        }
    
        if (!isset($_SESSION['selected_images'])) {
            $_SESSION['selected_images'] = [];
        }
        include '../views/gallery.php';
    }
    
    
    public function deleteImageAction() {
        if (!$this->userModel->isLoggedIn()) {
            header('Location: app.php?controller=user&action=loginAction');
            exit;
        }

        $imageId = $_POST['image_id'];
        $this->imageModel->deleteImage($imageId);
        header('Location: app.php?controller=gallery&action=galleryAction');
        exit;
    }
    public function viewImageAction() {
        $filename = $_GET['filename'];
        $image = $this->imageModel->getImageByFilename($filename);
        if (!$image) {
            header('Location: app.php?controller=gallery&action=galleryAction');
            exit;
        }
        include '../views/viewImage.php';
    }
    public function showSelectedAction() {
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['selected_images'])) {
            if (!isset($_SESSION['selected_images'])) {
                $_SESSION['selected_images'] = [];
            }
            
            $_SESSION['selected_images'] = array_unique(array_merge($_SESSION['selected_images'], $_POST['selected_images']));
        }
        $selectedImages = [];
        if (!empty($_SESSION['selected_images'])) {
            $selectedImages = $this->imageModel->getImagesByIds($_SESSION['selected_images']);
        }
    
        include '../views/showSelected.php';
    }
    
    public function deleteSelectedAction() {
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && !empty($_POST['selected_images'])) {
            $imagesToDeselect = $_POST['selected_images'];
            if (isset($_SESSION['selected_images']) && !empty($_SESSION['selected_images'])) {
                $_SESSION['selected_images'] = array_values(array_diff($_SESSION['selected_images'], $imagesToDeselect));
            }
        }
        header('Location: app.php?controller=gallery&action=showSelectedAction');
        exit;
    }
    
    
    public function searchAction() {
        $query = isset($_GET['query']) ? $_GET['query'] : '';
        
        $page = isset($_GET['page']) ? (int)$_GET['page'] : 1; 
        $limit = 8;
    
        if (empty($query)) {
            $publicImages = $this->imageModel->getPublicImages($page, $limit); // Pobiera wszystkie publiczne zdjęcia
        } else {
            $publicImages = $this->imageModel->searchImages($query); // Wyszukuje zdjęcia według tytułu lub autora
        }
    
        foreach ($publicImages as $image) {
            echo '<div class="image-card">
                    <a href="app.php?controller=gallery&action=viewImageAction&filename=' . urlencode($image['filename']) . '">
                        <img src="/images/' . basename($image['filename']) . '" alt="Image" width="200">
                    </a>
                    <h3>' . htmlspecialchars($image['title']) . '</h3>
                    <p>by: ' . htmlspecialchars($image['author']) . '</p>
                    <input type="checkbox" 
                           name="selected_images[]" 
                           value="' . $image['_id'] . '"' . 
                           (isset($_SESSION['selected_images']) && in_array($image['_id'], $_SESSION['selected_images']) ? ' checked' : '') . '>
                  </div>';
        }
    }
    
    
    
    
}
?>
