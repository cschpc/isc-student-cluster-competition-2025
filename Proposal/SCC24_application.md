# SCC24 proposal

## Institutional diversity

The body that is providing the training and funding for the team, CSC, is a non-profit scientific computing center in Finland.

CSC promotes non-discrimination, equality and diversity in its work community and to support this it has an equality plan that is based on the local legislation. The plan specifies the objectives set for the promotion of non-discrimination and equality and provides for their implementation, responsible persons, resources, follow-up and informing on them.

## Strength of Team

The current team Löyly (which translates to "Sauna heat") is a direct continuation of our previous efforts, team Revontuli (best HPL and third place in IndySCC 2022) and team Norppa (second place in IndySCC 2023). The team is coordinated and managed by CSC, which is the national supercomputing center of Finland and the host of the Top500 #5 LUMI pre-exascale supercomputer.

Even though all the team members are first-timers, we have retained the successful approach, the training program and the team advisor, Dr. Jussi Enkovaara, from the previous years. Additionally, a member of team Revontuli, Mr. Leopekka Saraste, has been working at CSC since Summer of 2022 and is utilizing his first-hand experiences from IndySCC in training the new team as a second advisor. Mr. Saraste acted as second advisor also for the team Norppa in 2023. Dr. Jussi Heikonen continues as the overall manager of CSC's SCC activities.

Being a computing center, CSC does not have a student program, so the students were recruited from two universities in the Helsinki metropolitan area: Aalto University and the University of Helsinki. CSC set up an open call for the competition and selected the students based on interviews from the most promising candidates. The applicants' technical skills, motivation to learn HPC, and team working skills were emphasized in the selection process. 

The team members have backgrounds in a variety of scientific disciplines, namely, in physics, computer science, and biological and environmental sciences. Some team members have prior HPC experience gained through summer traineeships at their university's research groups and through coursework, and all have strong Linux skills.
Specifically, members in the team have working experience in coding with OpenMP/MPI, in data analysis tasks or with Linux server administration. One of our team members (Kim Takala) has also studied and worked as an electrician prior to his University studies.
All team members are very interested in HPC, and are highly motivated to learn more. 

As the importance of HPC is increasing in a multitude of scientific disciplines, the competition is very likely to benefit the participants in whatever their exact academic path will turn out to be. 

The advisor, Dr. Jussi Enkovaara has background in computational physics and has been at CSC since 2005. He has experience in developing and optimizing large scientific applications in international HPC projects. He is also providing general HPC support and contributing to the CSC's user training. Since autumn 2022, he has carried main responsibility from porting the ICON earth system model to LUMI. In IndySCC22 and IndySCC23 he acted as an advisor for the highly successful "Team Revontuli" and "Team Norppa".

## Strength of Diversity

Our approach was to maximize the number of applications so that we could then form a diverse team. CSC made an open call for applications to two universities, Aalto University and University of Helsinki. Flyers were distributed to the universities, and the relevant professors and lecturers were contacted. Advertisements were sent to student union mailing lists. Diversity of applicants was emphasized in all of the advertisement materials. 

In Team Löyly and at CSC, we try to promote equality for applicants regarding gender, ethnic background and other factors to promote a more diverse and equal field in HPC for all students. 

We are happy to have a team together that consists of 3 male students and 3 female students and has diversity in the students' backgrounds. 
Our team consists of 4 Finnish students, one student from Italy and one Chinese student. International students can be underrepresented in these kinds of competitions in Finland, so we believe that this diversity in our backgrounds is a strength and a point of pride for our team. 

Additionally, we have students in the team who openly identify as LGBT, and some that are neurodivergent. Both of these groups are often underrepresented in the field of HPC.

As not all the members speak Finnish, the working language of the team is English.

## Strength of Hardware and Software Approach

We discussed the hardware choices with our vendor partner HPE and have ended up with the HPE ProLiant DL385 Gen11 Configure-to-order Server due to its high performance. The system consists of three dual socket compute nodes with two AMD EPYC 9354 32-core CPUs and two AMD MI210 GPUs each. Each GPU has about 20 TFLOPS/s double precision peak performance (over 40 TFLOPS/s double precision matrix performance). There is 768 GB (24 x 32 GB DDR5-4800) of memory per node and each GPU has 64 GB of HBM2e memory. For storage, there is an 3.2 TB NVMe drive in each server, making our total storage 9.6 TB in size. 

The system has the highest performing air cooled AMD GPUs and the CPUs are optimized for the competition requirements. The nodes are connected with the latest Slingshot interconnect which provides state-of-the art performance for inter-node communication.

As there is a power cap for the competition, we have considered all the related aspects. In full utilization, the system draws close to 4 kW, while in idle the power usage is around 1 kW. We expect to fine tune the GPU clock frequency and the fans to limit power consumption if needed, but we have made sure it fits under the 4500W power draw limit.

We have verified that all of the benchmarks (HPL/MLPerf) can be executed on the system. Regarding the applications, both NAMD and ICON are regularly run in LUMI in both CPU and GPU partitions. As our competition hardware and software environment is similar to LUMI, the applications can also be readily run there. Additionally, the team will be trained to convert CUDA codes to HIP if necessary, especially for the mystery application and for the reproducibility challenge. All porting efforts will be made available to the SCC committee. 

Furthermore, the team advisor, Dr. Enkovaara, has been the main developer for porting ICON to LUMI and thus provides the team with first-hand experience about the application.

The application codes and their dependencies will be installed with the applications' own build infrastructure (e.g. cmake/make) or with Spack when appropriate. The system will accommodate a set of compilers (Cray, AMD, GNU), and when preparing for the competition, we will investigate the optimal one for each application. The SLURM scheduler will be used for the job submissions on the system.

During the competition the team will use scripts prepared in advance to manage the application workflows. The system performance will be monitored continuously to spot any problems.

## Strength of Vendor/Institution Relationship

We are collaborating with HPE who is also the vendor for the LUMI supercomputer, so CSC has a very close relationship with them. HPE will provide us with two systems: one for the training period (located at HPE Grenoble and accessed remotely) and the competition system that will be available in June and eventually shipped to SC. The systems have identical architectures and, most importantly, the same GPUs and identical software stacks. This allows the team to work on the remote system to the last minute, and avoid possible international shipping problems.

HPE will provide training to the team regarding tuning the hardware, power management of the cluster, system initialization, system administration and also information about rebooting the system after shutdowns (e.g in case of an unexpected blackout).  

Additionally, we have a close relationship with AMD, and will be in contact with them about any sort of final tuning we can do for the GPUs and CPUs in our use.

Our main vendor partner, HPE, will cover the traveling costs for the team. 

## Team Preparation

With Team Löyly we follow the same successful approach as in 2022 and in 2023. Accordingly, we have been preparing for the competition with dedicated training sessions with hands-on exercises every third week, since February 2024. To complement the training, the team has been given homework assigments between the on-site sessions. 

The training topics include:
- Introduction to using HPC systems: Modules, Slurm, compiling and running applications
- Administrating HPC systems: Monitoring the system, installing new software
- Running and tuning the competition benchmarks
- Running and tuning the competition applications
- Deeper dive into HPC architectures: memory hierarchies, affinities, thread hierarchies in GPUs
- Cloud adminstration and deployment (if the team ends up in the IndySCC)

We will simulate the mystery application task with applications like Gromacs, Elmer or OpenFoam. As our system has AMD GPUs, training sessions will be held for porting CUDA codes to HIP, and preparing Make/CMake files in case the mystery application has only CUDA support. Finally, we will study the materials from the previous competitions and discuss the strategies that winning teams employed.

We also have a good contact with ETH Zürich's "Team RACKlette", and occasionally have had training sessions together. We belive that this sharing of information can make both teams stronger and bring networking opportunities for students on both sides.

In the training so far we have used LUMI (AMD MI250x GPUs), CSC's Mahti (1404 AMD Rome + 24 NVIDIA A100 GPU nodes) and Puhti (700 Intel CPU + 80 NVIDIA V100 GPU nodes) supercomputers. In the future, we will use also HPE's remote access platform with similar configuration to our competition system. Once the actual competition system is available, it will be exclusively used in the training.

The team members will have specific roles based on their individual strengths and interests (SysAdmins, Application specialists, HPL specialists etc.) and will be trained for these tasks in advance. However, the majority of the training is common to all team members, so that in principle, everybody knows how the cluster is managed and how all of the applications are run.

CSC employs a wide range of application specialists in HPC, and similarly to prior years, some of the experts that have worked on specific competition applications (Gromacs, NAMD, ICON) will occasionally give guest lectures and opportunities for the students to ask in-depth questions about these applications.
Additionally, the team will also benefit from Mr. Saraste's experience from the 2022 IndySCC. His experiences about working practices, task distribution and competition dynamics will be indispensable. 

We believe that it takes a village to build a team for SCC, and we believe that the training staff at CSC can provide all of the preparation necessary to give us students the best possible tools to succeed.

## Team Educational Goals

Each team member describes their goals below.


Based on a proven approach, CSC will write a recommendation of study credits for the students based on the time they spend working and preparing for SCC. In prior years, these have been accepted at the Universities with ease.
