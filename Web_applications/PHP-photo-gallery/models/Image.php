<?php
class Image {
    private $db;

    public function __construct($db) {
        $this->db = $db;
    }

    public function uploadImage($imageFile,$author,$title ,$watermarkText, $userId, $isPublic) {
        $targetDir = $_SERVER['DOCUMENT_ROOT'] . "/images/";

        $filename = basename($imageFile["name"]);
        
        if (!$isPublic) {
            $filename = $userId . '_' . $filename;
        }
        $targetFile = $targetDir . $filename;
        move_uploaded_file($imageFile["tmp_name"], $targetFile);

        $this->addWatermark($targetFile, $watermarkText);

        $this->db->images->insertOne([
            'filename' => $targetFile,
            'author' => $author,
            'title' => $title,
            'watermark' => $watermarkText,
            'user_id' => $userId,
            'status' => $isPublic ? 'public' : 'private'
        ]);
    }

private function addWatermark($imagePath, $text) {
    $image = imagecreatefrompng($imagePath);
    if (!$image) {
        die("Failed to load image.");
    }
    $white = imagecolorallocate($image, 255, 102, 204);
    $fontSize = 10;

    // size of the watermark text
    $textWidth = imagefontwidth($fontSize) * strlen($text);
    $textHeight = imagefontheight($fontSize);

    //image dimensions
    $imageWidth = imagesx($image);
    $imageHeight = imagesy($image);

    // position of the watermark
    $x = $imageWidth - $textWidth - 10;
    $y = $imageHeight - $textHeight - 10;

    //watermark text
    imagestring($image, $fontSize, $x, $y, $text, $white);
    imagepng($image, $imagePath);
    imagedestroy($image);
}

    public function getTotalPublicImagesCount() {
        $images = $this->db->images->find(['status' => 'public'])->toArray();
        return $images;
    }

    public function getPublicImages($page = 1, $limit = 8) {
        $skip = ($page - 1) * $limit;
        $images = $this->db->images->find(
            ['status' => 'public'], // filter public images
            [
                'skip' => $skip,
                'limit' => $limit
            ]
        )->toArray();
        
        return $images;
    }

    public function getPublicImageCount() {
        return $this->db->images->countDocuments(['status' => 'public']);
    }

    public function getPrivateImages($userId) {
        $images = $this->db->images->find(['user_id' => $userId, 'status' => 'private'])->toArray();
        return $images;
    }
    public function deleteImage($imageId) {
        $image = $this->db->images->findOne(['_id' => new MongoDB\BSON\ObjectId($imageId)]);
        
        if ($image) {
            $this->db->images->deleteOne(['_id' => new MongoDB\BSON\ObjectId($imageId)]);
            
            if (file_exists($image['filename'])) {
                unlink($image['filename']);
            }
        }
    }
    public function getImageByFilename($filename) {
        $image = $this->db->images->findOne(['filename' => $filename]);
        return $image ? $image : null;
    }
    public function getImagesByIds($ids) {
        if (empty($ids) || !is_array($ids)) {
            return [];
        }
        $objectIds = array_map(function($id) {
            return new MongoDB\BSON\ObjectId($id);
        }, $ids);
    
        $filter = ['_id' => ['$in' => $objectIds]];
        return $this->db->images->find($filter)->toArray();
    }
    public function searchImages($query) {
        $regexQuery = new MongoDB\BSON\Regex($query, 'i'); 
        $filter = [
            '$and' => [
                [
                    '$or' => [
                        ['title' => $regexQuery],
                        ['author' => $regexQuery]
                    ]
                ],
                ['status' => 'public']
            ]
        ];
    
        return $this->db->images->find($filter)->toArray();
    }
    public function getSearchImagesCount($searchQuery) {
        $regexQuery = new MongoDB\BSON\Regex($query, 'i');
        $filter = [
            '$and' => [
                [
                    '$or' => [
                        ['title' => $regexQuery],
                        ['author' => $regexQuery]
                    ]
                ],
                ['status' => 'public']
            ]
        ];
        return $this->db->images->count($filter);
    }
}
?>
