# Reference Paper 
## Sum up 
https://docs.google.com/spreadsheets/d/17i5cm190a3Yu4kfT59wX9f3g46_h_Q1Uq-wMZu_80qw/edit?usp=sharing  

## Simulation    
1. Learning Ground Traversability from Simulations  
https://arxiv.org/pdf/1709.05368.pdf  
  
2. Training a terrain traversability classifier for a planetary rover through simulation    
http://journals.sagepub.com/doi/pdf/10.1177/1729881417735401  


## Vision    
3. Traversability Classification for UGV Navigation:    
 A Comparison of Patch and Superpixel Representations    
http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=DCAE6FDE3DBB20D8576B408ECC4BA438?doi=10.1.1.92.358&rep=rep1&type=pdf    

> Their contribution: SuperPixel way to divide the terrain
> actually I only see traversability planning using patch, but actually there may happen some rapid terrain(especially in urban city, not natural environment), or some environment I  haven’t thought about.


## Traversability (mixed with learning, planning)  
4. Adaptive Traversability of Unknown Complex Terrain with Obstacles for Mobile Robots  
http://cmp.felk.cvut.cz/~zimmerk/zimmermann-ICRA-2014.pdf  
> Their Contribution: AT (Adaptive Traversability), 


5. Traversability Classification using Unsupervised On-line Visual Learning for Outdoor Robot Navigation   
http://sangminoh.org/Publications_files/Kim06icra.pdf



6. Terrain Classification for Mobile Robots Traveling at Various Speeds: An Eigenspace Manifold Approach  
https://pdfs.semanticscholar.org/2c10/66fad44040366fa8f315c2eb0e11de666351.pdf

7. Navigation Planning for Legged Robots in Challenging Terrain  
https://drive.google.com/drive/folders/1d8PaoyMBMX9-5k4gHnED3Ku-qu_v8pCK

---

### MORE ABOUT defining Traversability  
8. Stereo Camera Based Navigation of Mobile Robots on Rough Terrain  
http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.454.9074&rep=rep1&type=pdf

9. Combining Vision Learning and Interaction for Mobile Robot Path Planning   
http://journals.sagepub.com/doi/pdf/10.5772/50827




## About dataset
### Terrain generation:(Grid map 還沒使用過）
### Grid Map    
ETHZ ASL  
may use ETH ASL github  
https://github.com/ethz-asl/grid_map

### height map  
<>
### blender  
<>
>我們現在的terrain 一種是用blender 手動random每個patch 另一種是用height map 的方式（只要有height map 的gray image, gazebo 有辦法生出整個height map 立體模型, DRC, SRC 的場景都是這樣生出來的, simulation 第一篇論文也是用height map這個方法）

 
