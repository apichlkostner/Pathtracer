import numpy as np
import random
import matplotlib.pyplot as plt
from multiprocessing import Process, Queue
import os

def random_in_unit_sphere():
  while(True):
    v = np.array([random.uniform(-1., 1.), random.uniform(-1., 1.), random.uniform(-1., 1.)])
    if (np.linalg.norm(v) <= 1.):
      return v

def random_unit_vector():
  a = random.uniform(0., 2. * np.pi)
  z = random.uniform(-1., 1.)
  r = np.sqrt(1. - z*z)

  return np.array([r*np.cos(a), r*np.sin(a), z])

def update_hits(random_function, hits, normal_vector):
  v = random_function()
  angle = np.arccos(v.dot(normal_vector))
  bucket = int(angle / np.pi * num_buckets)
  hits[bucket] += 1

def iterate_random_function(random_function, hits, normal_vector, num_iterations, q):
  print("Started process {}".format(os.getpid()))
  for _ in range(num_iterations):
    update_hits(random_function, hits, normal_vector)
  print("Finished process {}".format(os.getpid()))
  q.put(hits)

if __name__ == "__main__":
  num_iterations = 1000000
  num_buckets = 1000
  hits = np.zeros(num_buckets)
  hits_unit_sphere = np.zeros(num_buckets)
  cos_fun = np.zeros(num_buckets)
  normal_vector = np.array([0., 0., 1.])

  parallel_processing = True
  do_plot = True

  if (parallel_processing):
    q_vector = Queue()
    q_sphere = Queue()
    processes = []

    # calculate in different processes
    processes.append(Process(target=iterate_random_function, args=(random_unit_vector, hits, normal_vector, num_iterations, q_vector)))
    processes.append(Process(target=iterate_random_function, args=(random_in_unit_sphere, hits_unit_sphere, normal_vector, num_iterations, q_sphere)))

    for p in processes:
      p.start()

    for p in processes:
      p.join()

    hits = q_vector.get()
    hits_unit_sphere = q_sphere.get()
  else:    
    for _ in range(num_iterations):
      # update hits with value from random unit vector function
      update_hits(random_unit_vector, hits, normal_vector)
      # update hits with value from random in unit sphere
      update_hits(random_in_unit_sphere, hits_unit_sphere, normal_vector)
    
  # generate plot

  # x axis
  x_from = -np.pi / 2.
  x_to   = +np.pi / 2.
  x_len  = x_to - x_from
  x_axis = np.arange(x_from, x_to, x_len / num_buckets)
  
  # normalize
  prob_density_unit_vector = (hits / num_iterations) * num_buckets / np.pi
  prob_density_unit_sphere = (hits_unit_sphere / num_iterations) * num_buckets / np.pi
  # integral cos(x) [-pi/2, +pi/2] = 2
  cos_function_normalized = np.cos(x_axis) / 2.
  # integral cos3(x) [-pi/2, +pi/2] = 4/3
  cos3_function_normalized = np.cos(x_axis) * np.cos(x_axis) * np.cos(x_axis) * 3. / 4.
  
  # simple numerical integration as sanity check
  integral_unit_vector = np.sum(prob_density_unit_vector) * x_len / num_buckets
  integral_unit_sphere = np.sum(prob_density_unit_sphere) * x_len / num_buckets
  integral_cos = np.sum(cos_function_normalized) * x_len / num_buckets
  integral_cos3 = np.sum(cos3_function_normalized) * x_len / num_buckets

  # check that integrals are 1.0
  print("Integral of random function by unit_vector = {:.2}".format(integral_unit_vector))
  print("Integral of random function by unit_sphere = {:.2}".format(integral_unit_sphere))
  print("Integral of random function by cos = {:.2}".format(integral_cos))
  print("Integral of random function by cos³ = {:.2}".format(integral_cos3))
  
  # plot the generated functions and the cos reference
  if (do_plot):
    plt.plot(x_axis, prob_density_unit_vector, 'b')
    plt.plot(x_axis, prob_density_unit_sphere, 'r')
    plt.plot(x_axis, cos_function_normalized, 'g', linewidth=1)
    plt.plot(x_axis, cos3_function_normalized, 'y', linewidth=1)
    plt.show()

