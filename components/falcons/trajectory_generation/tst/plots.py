#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt



def make_plots(json_data, dofs):
    samples = json_data['output']['samples']
    target = json_data['input']['setpoint']
    t = [sample['t'] for sample in samples]
    x = [sample['position']['x'] for sample in samples]
    y = [sample['position']['y'] for sample in samples]
    rz = [sample['position']['rz'] for sample in samples]
    dx = [sample['position']['x'] - target['position']['x'] for sample in samples]
    dy = [sample['position']['y'] - target['position']['y'] for sample in samples]
    drz = [sample['position']['rz'] - target['position']['rz'] for sample in samples] # TODO unwrap backwards
    vx = [sample['velocity']['x'] for sample in samples]
    vy = [sample['velocity']['y'] for sample in samples]
    vrz = [sample['velocity']['rz'] for sample in samples]
    dvx = [sample['velocity']['x'] - target['velocity']['x'] for sample in samples]
    dvy = [sample['velocity']['y'] - target['velocity']['y'] for sample in samples]
    dvrz = [sample['velocity']['rz'] - target['velocity']['rz'] for sample in samples] # TODO unwrap backwards
    # TODO: this was not the most computationally efficient way of coding it, but the number of samples is expected to be small

    # figure 1: field trajectory (no time stamps)
    plt.figure(1)
    plt.plot(x, y, 'b+', label='robot.pos')
    # TODO: maybe show orientation as little lines/arrows?
    plt.title('field plot (xy)')
    plt.xlabel('x [m]')
    plt.ylabel('y [m]')
    ax = plt.gca()
    ax.legend()
    ax.grid(True)

    # figure 2: data over time
    # plot delta to a) always converge to 0 and b) avoid angle jumps
    plt.figure(2)
    if 'x' in dofs:
        plt.plot(t, dx, 'b+-', label='delta.pos.x [m]')
        plt.plot(t, dvx, 'bx:', label='delta.vel.x [m/s]')
    if 'y' in dofs:
        plt.plot(t, dy, 'r+-', label='delta.pos.y [m]')
        plt.plot(t, dvy, 'rx:', label='delta.vel.y [m/s]')
    if 'rz' in dofs:
        plt.plot(t, drz, 'm+-', label='delta.pos.rz [rad]')
        plt.plot(t, dvrz, 'mx:', label='delta.vel.rz [rad/s]')
    plt.title('timeline plot (xy)')
    plt.xlabel('t [sec]')
    ax = plt.gca()
    ax.legend()
    ax.grid(True)
    plt.show()

