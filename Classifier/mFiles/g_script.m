figure(1);
nch = 14;
nt=6;
for tri=1:nt
    for chi=1:nch
    subplot(3,2,tri)
    hold on;
    plot(t*1000,eegData(:,chi,tri))
    title(['trial ' num2str(tri)])
    ax = axis;
    axis([-500 0 ax(3) ax(4)])
    xlabel('Time (ms)')
    end
end